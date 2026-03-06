// 预先导入和绑定常用模块及函数
// const fs = require("fs");
// path = require("path");
// const tmpPath = require('os').tmpdir();
const encrypt = require("@NeteaseCloudMusicApi/util/crypto");
const CryptoJS = require("crypto-js");
const { default: axios } = require("axios");
const {
  cookieToJson,
  cookieObjToString,
  toBoolean,
  generateRandomChineseIP,
} = require("@NeteaseCloudMusicApi/util/index");
// const { URLSearchParams, URL } = require("url");
const { APP_CONF } = require("@NeteaseCloudMusicApi/util/config.json");

function URLSearchParams(obj) {
  return Object.keys(obj)
    .map(
      (key) =>
        encodeURIComponent(key) + "=" + encodeURIComponent(obj[key])
    )
    .join("&");
}

// 预先读取匿名token并缓存
// const anonymous_token = fs.readFileSync(
//   path.resolve(tmpPath, './anonymous_token'),
//   'utf-8',
// )

// console.log(`[INIT] Anonymous token loaded: ${anonymous_token ? 'Yes' : 'No'}`);
// console.log(`[INIT] Anonymous token value: ${anonymous_token}`);

const anonymous_token = process.env.ANONYMOUS_TOKEN || "";
// const anonymous_token = '00A84A3B4FFDD2F457374924C7E1750E07B5047E045E658C57AA0CBD375D99FE8662F81007305FF09AC6B4516D522331B739675ABE93B455C5CE256C19E041CEC22ED40AA2C8A5314E340F289955BEB2BDACD006F779828452488214B77E343B9FE0CD59632EE6755F9EB4D5B4CE8080EC180D071BC47F86D86C32A13329901BDCE9B9F9E4A05D447BACCA5C834CC4A03F0BD666C9974062F95AA8DEA302BC29045D8CE209B3903168128CF4FC6250710E67B46307A1E628FB5A8288BE6544820889018347C6F2B0949245533D9A9B39A2EBCE18BC7099DC42E44720F0DC7585D532379ACAB1AA71676720C6F53F078B70F5AC335BA221BF6BB954ABDE714FA0335A440DB4CCBAA2F0E73D9383CC36BE9861BF88DB17A0B854F36EC4B10EEF838B38BF802F10EEFB851EFC92BC447C425D106F6BC4BB2F4028AACC5C1BFADF285621DA0FD33F004FA58ADC5A876FB0C808CA3DFCE6B9FCAFD424464CA0612819C960FF74C1DA449691F1159D26F1D3F555BE563BF9CDD75FA980F7EE7A2483170F8F92737C10B095A893E99A8C3DDFA3E6B486CEBC12EEE3E21E204AAAB4FD4032398CBFD41245E5BBA24DE114D02D925B62AFE962517EBBD2C286376359EE553E9CEBB5B3C6196154A6EC0B60F1D0EB81B109A0D032249DA2331C3C617A6F6ABA7DF7212BFAA97411623A3690DACE8E1F569B59D891E00BDB60B9DBE883F682C6403ECC8FE79B20CBC6A145D72BDC77B3';


console.log(`[INIT] Anonymous token loaded: ${anonymous_token ? 'Yes' : 'No'}`);
console.log(`[INIT] Anonymous token value: ${anonymous_token}`);

// 预先绑定常用函数和常量
const floor = Math.floor;
const random = Math.random;
const now = Date.now;
const keys = Object.keys;
const stringify = JSON.stringify;
const parse = JSON.parse;
const characters = "abcdefghijklmnopqrstuvwxyz";
const charactersLength = characters.length;

// // 预先创建HTTP/HTTPS agents并重用
// const createHttpAgent = () => new http.Agent({ keepAlive: true })
// const createHttpsAgent = () => new https.Agent({ keepAlive: true })

// 预先计算WNMCID（只计算一次）
const WNMCID = (function () {
  let randomString = "";
  for (let i = 0; i < 6; i++) {
    randomString += characters.charAt(floor(random() * charactersLength));
  }
  return `${randomString}.${now().toString()}.01.0`;
})();

// 预先定义osMap
const osMap = {
  pc: {
    os: "pc",
    appver: "3.1.17.204416",
    osver: "Microsoft-Windows-10-Professional-build-19045-64bit",
    channel: "netease",
  },
  linux: {
    os: "linux",
    appver: "1.2.1.0428",
    osver: "Deepin 20.9",
    channel: "netease",
  },
  android: {
    os: "android",
    appver: "8.20.20.231215173437",
    osver: "14",
    channel: "xiaomi",
  },
  iphone: {
    os: "iPhone OS",
    appver: "9.0.90",
    osver: "16.2",
    channel: "distribution",
  },
};

// 预先定义userAgentMap
const userAgentMap = {
  weapi: {
    pc: "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36 Edg/124.0.0.0",
  },
  linuxapi: {
    linux:
      "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36",
  },
  api: {
    pc: "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Safari/537.36 Chrome/91.0.4472.164 NeteaseMusicDesktop/3.0.18.203152",
    android:
      "NeteaseMusic/9.1.65.240927161425(9001065);Dalvik/2.1.0 (Linux; U; Android 14; 23013RK75C Build/UKQ1.230804.001)",
    iphone: "NeteaseMusic 9.0.90/5038 (iPhone; iOS 16.2; zh_CN)",
  },
};

// 预先定义常量
const DOMAIN = APP_CONF.domain;
const API_DOMAIN = APP_CONF.apiDomain;
const ENCRYPT_RESPONSE = APP_CONF.encryptResponse;
const SPECIAL_STATUS_CODES = new Set([201, 302, 400, 502, 800, 801, 802, 803]);

// chooseUserAgent函数
const chooseUserAgent = (crypto, uaType = "pc") => {
  return (userAgentMap[crypto] && userAgentMap[crypto][uaType]) || "";
};

// cookie处理
const processCookieObject = (cookie, uri) => {
  const _ntes_nuid = CryptoJS.lib.WordArray.random(32).toString();
  const os = osMap[cookie.os] || osMap["pc"];

  const processedCookie = {
    ...cookie,
    __remember_me: "true",
    ntes_kaola_ad: "1",
    _ntes_nuid: cookie._ntes_nuid || _ntes_nuid,
    _ntes_nnid: cookie._ntes_nnid || `${_ntes_nuid},${now().toString()}`,
    WNMCID: cookie.WNMCID || WNMCID,
    WEVNSM: cookie.WEVNSM || "1.0.0",
    osver: cookie.osver || os.osver,
    deviceId: cookie.deviceId || global.deviceId,
    os: cookie.os || os.os,
    channel: cookie.channel || os.channel,
    appver: cookie.appver || os.appver,
  };

  if (uri.indexOf("login") === -1) {
    processedCookie["NMTID"] = CryptoJS.lib.WordArray.random(16).toString();
  }

  if (!processedCookie.MUSIC_U) {
    processedCookie.MUSIC_A = processedCookie.MUSIC_A || anonymous_token;
  }

  return processedCookie;
};

// header cookie生成
const createHeaderCookie = (header) => {
  const headerKeys = keys(header);
  const cookieParts = new Array(headerKeys.length);

  for (let i = 0, len = headerKeys.length; i < len; i++) {
    const key = headerKeys[i];
    cookieParts[i] =
      encodeURIComponent(key) + "=" + encodeURIComponent(header[key]);
  }

  return cookieParts.join("; ");
};

// requestId生成
const generateRequestId = () => {
  return `${now()}_${floor(random() * 1000)
    .toString()
    .padStart(4, "0")}`;
};

const createRequest = (uri, data, options) => {
  // console.log(
  //   `[REQUEST] uri: ${uri}, data: ${JSON.stringify(
  //     data
  //   )}, options: ${JSON.stringify(options)}`
  // );
  return new Promise((resolve, reject) => {
    // 变量声明和初始化
    const headers = options.headers ? { ...options.headers } : {};
    const ip = options.realIP || options.ip || "";

    // IP头设置
    if (ip) {
      headers["X-Real-IP"] = ip;
      headers["X-Forwarded-For"] = ip;
    }

    let cookie = options.cookie || {};
    if (typeof cookie === "string") {
      cookie = cookieToJson(cookie);
    }

    if (typeof cookie === "object") {
      cookie = processCookieObject(cookie, uri);
      headers["Cookie"] = cookieObjToString(cookie);
    }
    let url = "";
    let encryptData = "";
    let crypto = options.crypto;
    const csrfToken = cookie["__csrf"] || "";

    // console.log(
    //   `[REQUEST] selected crypto: APP_CONF.encrypt: ${JSON.stringify(
    //     APP_CONF.encrypt
    //   )} `
    // );

    // 加密方式选择
    if (crypto === "") {
      crypto = APP_CONF.encrypt ? "eapi" : "api";
    }

    const answer = { status: 500, body: {}, cookie: [] };

    try {
      // 根据加密方式处理
      switch (crypto) {
        case "weapi":
          headers["Referer"] = options.domain || DOMAIN;
          headers["User-Agent"] = options.ua || chooseUserAgent("weapi");
          data.csrf_token = csrfToken;
          encryptData = encrypt.weapi(data);
          url = (options.domain || DOMAIN) + "/weapi/" + uri.substr(5);
          break;

        case "linuxapi":
          headers["User-Agent"] =
            options.ua || chooseUserAgent("linuxapi", "linux");
          encryptData = encrypt.linuxapi({
            method: "POST",
            url: (options.domain || DOMAIN) + uri,
            params: data,
          });
          url = (options.domain || DOMAIN) + "/api/linux/forward";
          break;

        case "eapi":
        case "api":
          // header创建
          const header = {
            osver: cookie.osver,
            deviceId: cookie.deviceId,
            os: cookie.os,
            appver: cookie.appver,
            versioncode: cookie.versioncode || "140",
            mobilename: cookie.mobilename || "",
            buildver: cookie.buildver || now().toString().substr(0, 10),
            resolution: cookie.resolution || "1920x1080",
            __csrf: csrfToken,
            channel: cookie.channel,
            requestId: generateRequestId(),
            ...(options.checkToken
              ? { "X-antiCheatToken": APP_CONF.checkToken }
              : {}),
            // clientSign: APP_CONF.clientSign,
          };

          if (cookie.MUSIC_U) header["MUSIC_U"] = cookie.MUSIC_U;
          if (cookie.MUSIC_A) header["MUSIC_A"] = cookie.MUSIC_A;

          headers["Cookie"] = createHeaderCookie(header);
          headers["User-Agent"] =
            options.ua || chooseUserAgent("api", "iphone");

          if (crypto === "eapi") {
            data.header = header;
            data.e_r = toBoolean(
              options.e_r !== undefined
                ? options.e_r
                : data.e_r !== undefined
                ? data.e_r
                : ENCRYPT_RESPONSE
            );
            encryptData = encrypt.eapi(uri, data);
            url = (options.domain || API_DOMAIN) + "/eapi/" + uri.substr(5);
          } else if (crypto === "api") {
            url = (options.domain || API_DOMAIN) + uri;
            encryptData = data;
          }
          break;

        default:
          console.log("[ERR]", "Unknown Crypto:", crypto);
          break;
      }
    } catch (e) {
      console.log("Error during anonymous registration:", e);
      console.log("Error stack:", e && e.stack);
      console.log("Error JSON:", JSON.stringify(e));

      reject({
        status: 500,
        body: { code: 500, msg: "Error during request setup" },
        cookie: [],
      });
    }
    // console.log(url);
    // settings创建
    let settings = {
      method: "POST",
      url: url,
      headers: headers,
      data: URLSearchParams(encryptData).toString(),
      // httpAgent: createHttpAgent(),
      // httpsAgent: createHttpsAgent(),
    };

    // e_r处理
    if (data.e_r) {
      settings.encoding = null;
      settings.responseType = "arraybuffer";
      // console.log(
      //   `[REQUEST] e_r is true, setting responseType to arraybuffer and encoding to null`
      // );
    }

    // // 代理处理
    // if (options.proxy) {
    //   if (options.proxy.indexOf('pac') > -1) {
    //     const agent = new PacProxyAgent(options.proxy)
    //     settings.httpAgent = agent
    //     settings.httpsAgent = agent
    //   } else {
    //     try {
    //       const purl = new URL(options.proxy)
    //       if (purl.hostname) {
    //         const isHttps = purl.protocol === 'https:'
    //         const agent = tunnel[isHttps ? 'httpsOverHttp' : 'httpOverHttp']({
    //           proxy: {
    //             host: purl.hostname,
    //             port: purl.port || 80,
    //             proxyAuth:
    //               purl.username && purl.password
    //                 ? purl.username + ':' + purl.password
    //                 : '',
    //           },
    //         })
    //         settings.httpsAgent = agent
    //         settings.httpAgent = agent
    //         settings.proxy = false
    //       } else {
    //         console.error('代理配置无效,不使用代理')
    //       }
    //     } catch (e) {
    //       console.error('代理URL解析失败:', e.message)
    //     }
    //   }
    // } else {
    //   settings.proxy = false
    // }
    // console.log(settings.headers);
    // console.log(
    //   "[REQ]",
    //   JSON.stringify(settings)
    // );
    axios(settings)
      .then((res) => {
        // console.log(
        //   "[RES]",
        //   JSON.stringify({
        //     url: settings.url,
        //     status: res.status,
        //     headers: res.headers,
        //   })
        // );
        const body = res.data;
        const rawCookie = res.headers["set-cookie"];
        let cookieArr = [];
        if (Array.isArray(rawCookie)) {
          cookieArr = rawCookie;
        } else if (typeof rawCookie === "string" && rawCookie) {
          // 只在 ", CookieName=" 边界处切割，避免误切 Expires 日期中的逗号
          cookieArr = rawCookie.split(/,\s*(?=[A-Za-z_][A-Za-z0-9_]*=)/);
        }
        answer.cookie = cookieArr.map((x) =>
          x.replace(/\s*Domain=[^(;|$)]+;*/, "")
        );

        try {
          if (crypto === "eapi" && data.e_r) {
            answer.body = encrypt.eapiResDecrypt(
              body.toString("hex").toUpperCase()
            );
          } else {
            answer.body =
              typeof body === "object" ? body : parse(body.toString());
          }

          if (answer.body.code) {
            answer.body.code = Number(answer.body.code);
          }

          answer.status = Number(answer.body.code || res.status);

          // 状态码检查（使用Set提升查找性能）
          if (SPECIAL_STATUS_CODES.has(answer.body.code)) {
            answer.status = 200;
          }
        } catch (e) {
          answer.body = body;
          answer.status = res.status;
        }

        answer.status =
          answer.status > 100 && answer.status < 600 ? answer.status : 400;

        if (answer.status === 200) {
          resolve(answer);
        } else {
          console.log("[ERR]", answer);
          reject(answer);
        }
      })
      .catch((err) => {
        answer.status = 502;
        answer.body = { code: 502, msg: err.message || err };
        console.log("[ERR]", answer);
        reject(answer);
      });
  });
};

module.exports = createRequest;
