const { cookieToJson, randomString, getGuid, calculateMid } = require('@kuGouMusicApi/util/util');
const { cryptoMd5 } = require('@kuGouMusicApi/util/crypto');
const { createRequest } = require('@kuGouMusicApi/util/request');
const moduleDefinitions = require('./modulesDefinitions'); // 由 webpack.config.js 生成的模块定义文件

const guid = cryptoMd5(getGuid());
const serverDev = randomString(10).toUpperCase();

const axios = require('axios');

class KuGouMusicApi {
  constructor() {
    this.axios = axios;
  }
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
    .replace(/^\//, '')
    .split('/')
    .map((seg, idx) => (idx === 0 ? seg : seg.charAt(0).toUpperCase() + seg.slice(1)))
    .join('');
};

// 记录 route 到模块的映射
const routeModuleMap = {};
moduleDefinitions.forEach(({ route, module }) => {
  const methodName = routeToMethodName(route);
  routeModuleMap[route] = module;
  KuGouMusicApi.prototype[methodName] = function (...args) {
    return module.apply(this, args);
  };
});

// 统一的 route 方法
KuGouMusicApi.prototype.route = async function (route, _cookie, args) {
  let params = {};
  try {
    params = JSON.parse(args || '{}');
  } catch (e) {
    // 如果解析失败，保持原样
    console.warn('Failed to parse params, using empty object:', args);
  }
  console.log(`[ROUTE] route: ${route}, cookie: ${_cookie}, params: ${JSON.stringify(params)}`);
  // args 是键值对形式，如 route, cookie, 'a', 1, 'b', 2
  for (let i = 0; i < args.length; i += 2) {
    if (typeof args[i] === 'string') {
      params[args[i]] = args[i + 1];
    }
  }

  const module = routeModuleMap[route];
  if (typeof module === 'function') {
    // 如果传入 cookie 是字符串，转换为对象
    let req = {};
    let res = new Res();
    req.cookies = {};
    req.ip = '192.168.5.7'; // Todo

    if (typeof _cookie === 'string') {
      _cookie
        .trim()
        .split(/;\s+/g)
        .forEach((pair) => {
          const crack = pair.indexOf('=');
          if (crack < 1 || crack === pair.length - 1) {
            return;
          }
          // req.cookies[decode(pair.slice(0, crack)).trim()] = decode(pair.slice(crack + 1)).trim();
          const rawKey = pair.slice(0, crack);
          const rawVal = pair.slice(crack + 1);
          const key = decodeURIComponent(rawKey).trim();
          const val = decodeURIComponent(rawVal).trim();
          req.cookies[key] = val;
        });
    } else if (typeof _cookie === 'object' && _cookie !== null) {
      req.cookies = _cookie;
    }

    const cookies = req.cookies || {};
    req.protocol = 'https'; // 默认协议，可以根据需要调整 Todo
    const isHttps = req.protocol === 'https';
    const cookieSuffix = isHttps ? '; PATH=/; SameSite=None; Secure' : '; PATH=/';

    const ensureCookie = (key, value) => {
      if (Object.prototype.hasOwnProperty.call(cookies, key)) return;
      cookies[key] = String(value);
      res.append('Set-Cookie', `${key}=${cookies[key]}${cookieSuffix}`);
    };

    const mid = calculateMid(process.env.KUGOU_API_GUID ?? guid);
    ensureCookie('KUGOU_API_PLATFORM', process.env.platform);
    ensureCookie('KUGOU_API_MID', mid);
    ensureCookie('KUGOU_API_GUID', process.env.KUGOU_API_GUID ?? guid);
    ensureCookie('KUGOU_API_DEV', (process.env.KUGOU_API_DEV ?? serverDev).toUpperCase());
    ensureCookie('KUGOU_API_MAC', (process.env.KUGOU_API_MAC ?? '02:00:00:00:00:00').toUpperCase());

    req.cookies = cookies;

    const query = Object.assign({}, { cookie: req.cookies }, params, { body: {} });

    // Todo: 缓存 app.use(cache('2 minutes', (_, res) => res.statusCode === 200));

    // const moduleResponse =
    try {
      const moduleResponse = await module(query, (config) => {
        let ip = req.ip;
        if (ip.substring(0, 7) === '::ffff:') {
          ip = ip.substring(7);
        }
        // config.ip = ip;
        config.ip = '::1'; // Todo
        return createRequest(config);
      });

      const cookies = moduleResponse.cookie;
      if (!query.noCookie) {
        if (Array.isArray(cookies) && cookies.length > 0) {
          if (req.protocol === 'https') {
            // Try to fix CORS SameSite Problem
            res.append(
              'Set-Cookie',
              cookies.map((cookie) => {
                return `${cookie}; PATH=/; SameSite=None; Secure`;
              })
            );
          } else {
            res.append(
              'Set-Cookie',
              cookies.map((cookie) => {
                return `${cookie}; PATH=/`;
              })
            );
          }
        }
      }

      res.header(moduleResponse.headers).status(moduleResponse.status).send(moduleResponse.body);

      return JSON.stringify(res.toObject());
    } catch (e) {
      const moduleResponse = e;
      console.log(`[ERR] ${route} ${JSON.stringify(moduleResponse)}`);

      if (!moduleResponse.body) {
        res.status(404).send({
          code: 404,
          data: null,
          msg: 'Not Found',
        });
        return;
      }

      res.header(moduleResponse.headers).status(moduleResponse.status).send(moduleResponse.body);
      return JSON.stringify(res.toObject());
    }
  } else {
    throw new Error(`No module found for route: ${route}`);
  }
};

// 动态挂载所有模块方法到 KuGouMusicApi 原型
moduleDefinitions.forEach(({ route, module }) => {
  const methodName = routeToMethodName(route);
  KuGouMusicApi.prototype[methodName] = async function (cookie, args) {
    const result = KuGouMusicApi.prototype.route.call(this, route, cookie, args);
    console.log(`[${methodName}] result:`, result);
    return result;
  };
});

module.exports = KuGouMusicApi;