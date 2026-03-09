// const path = require("path");
// const express = require('express')
const request = require("./request");
const packageJSON = require("@NeteaseCloudMusicApi/package.json");
const cache = require("@NeteaseCloudMusicApi/util/apicache").middleware;
const { cookieToJson } = require("@NeteaseCloudMusicApi/util/index");
const { generateRandomChineseIP } = require("@NeteaseCloudMusicApi/util/index");
const moduleDefinitions = require("./modulesDefinitions.js");

/**
 * @typedef {{
 *   identifier?: string,
 *   route: string,
 *   module: any
 * }} ModuleDefinition
 */

/**
 * @typedef {{
 *   port?: number,
 *   host?: string,
 *   checkVersion?: boolean,
 *   moduleDefs?: ModuleDefinition[]
 * }} NcmApiOptions
 */

const axios = require("axios");

class NeteaseCloudMusicApiApi {
  static generateRandomChineseIP = generateRandomChineseIP;
}

class Res {
  constructor() {
    this.headers = {};
    this.statusCode = 200;
    this.body = null;
    this.cookies = [];
  }

  append(header, value) {
    // 支持多值头部
    if (!this.headers[header]) {
      this.headers[header] = [];
    }
    if (Array.isArray(value)) {
      this.headers[header].push(...value);
    } else {
      this.headers[header].push(value);
    }
  }

  header(headers) {
    Object.assign(this.headers, headers);
    return this;
  }

  status(code) {
    this.statusCode = code;
    return this;
  }

  send(body) {
    this.body = body;
    return this;
  }

  toObject() {
    return {
      headers: this.headers,
      status: this.statusCode,
      body: this.body,
    };
  }
}

// 路由到方法名映射
const routeToMethodName = (route) => {
  return route
    .replace(/^\//, "")
    .split("/")
    .map((seg, idx) =>
      idx === 0 ? seg : seg.charAt(0).toUpperCase() + seg.slice(1)
    )
    .join("");
};

// 记录 route 到模块的映射
const routeModuleMap = {};
moduleDefinitions.forEach(({ route, module }) => {
  const methodName = routeToMethodName(route);
  routeModuleMap[route] = module;
  NeteaseCloudMusicApiApi.prototype[methodName] = function (...args) {
    return module.apply(this, args);
  };
});

NeteaseCloudMusicApiApi.prototype.get_anonimous_cookie = async () => {
  try {
    const register_anonimous = function (data = {}) {
      const cookie =
        typeof data.cookie === "string"
          ? cookieToJson(data.cookie)
          : data.cookie || {};
      return routeModuleMap['/register/anonimous'](
        {
          ...data,
          cookie,
        },
        async (...args) => {
          return request(...args);
        }
      );
    };

    const res = await register_anonimous();
    const cookie = res.body.cookie;
    const cookieObj = cookieToJson(cookie);
    const MUSIC_A = cookieObj.MUSIC_A || "";

    return MUSIC_A;
  } catch (e) {
    console.log("Error during anonymous registration:", JSON.stringify(e));
    console.log("Error stack:", e && e.stack);
    console.log("Error JSON:", JSON.stringify(e));
    throw e;
  }
};

// 统一的 route 方法
NeteaseCloudMusicApiApi.prototype.route = async function (
  route,
  _cookie,
  args
) {
  /**
   * _cookie 和 args 都是json字符串，需要解析成对象
   */
  let req = {};
  let res = new Res();
  req.cookies = {};
  req.ip = "192.168.5.7"; // Todo
  let params = {};
  try {
    params = JSON.parse(args || "{}");
  } catch (e) {
    // 如果解析失败，保持原样
    console.warn("Failed to parse params, using empty object:", args);
  }

  try {
    req.cookies = JSON.parse(_cookie || "{}");
  } catch (e) {
    console.warn("Failed to parse cookies, using empty object:", args);
  }
  console.log(
    `[ROUTE] route: ${route}, cookie: ${_cookie}, params: ${JSON.stringify(
      params
    )}`
  );
 
  const module = routeModuleMap[route];
  if (typeof module === "function") {
    req.ip = "192.168.5.7"; // Todo

    const query = Object.assign({}, { cookie: req.cookies }, params, {
      body: {},
    });

    // Todo: 缓存 app.use(cache('2 minutes', (_, res) => res.statusCode === 200));

    // const moduleResponse =
    try {
      const moduleResponse = await module(query, (...params) => {
        // 参数注入客户端IP
        const obj = [...params];
        let ip = req.ip;

        if (ip.substring(0, 7) == "::ffff:") {
          ip = ip.substring(7);
        }
        if (ip == "::1") {
          ip = process.env.cnIp; // 统一从环境变量获取
        }
        // console.log(ip)
        obj[3] = {
          ...obj[3],
          ip,
        };
        return request(...obj);
      });

      const cookies = moduleResponse.cookie;
      if (!query.noCookie) {
        if (Array.isArray(cookies) && cookies.length > 0) {
          if (req.protocol === "https") {
            // Try to fix CORS SameSite Problem
            res.append(
              "Set-Cookie",
              cookies.map((cookie) => {
                return `${cookie}; PATH=/; SameSite=None; Secure`;
              })
            );
          } else {
            res.append(
              "Set-Cookie",
              cookies.map((cookie) => {
                return `${cookie}; PATH=/`;
              })
            );
          }
        }
      }

      res
        .header(moduleResponse.headers)
        .status(moduleResponse.status)
        .send(moduleResponse.body);

      return JSON.stringify(res.toObject());
    } catch (e) {
      const moduleResponse = e;
      console.log(`[ERR] ${route} ${JSON.stringify(moduleResponse)}`);

      if (!moduleResponse.body) {
        res.status(404).send({
          code: 404,
          data: null,
          msg: "Not Found",
        });
        return;
      }

      res
        .header(moduleResponse.headers)
        .status(moduleResponse.status)
        .send(moduleResponse.body);
      return JSON.stringify(res.toObject());
    }
  } else {
    throw new Error(`No module found for route: ${route}`);
  }
};

module.exports = NeteaseCloudMusicApiApi;
