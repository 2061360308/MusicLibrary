/**
 * 劫持axios请求，由外部负责处理请求
 */
class AxiosHijack {
  // constructor() {
  //   this.queue = [];
  // }

  // setResponse(requestId, { data, status, statusText, headers, request, config }) {
  //   const item = this.queue.find((x) => x.id === requestId);
  //   if (item) {
  //     item.response = {
  //       data,
  //       status: typeof status === 'number' ? status : 200,
  //       statusText: statusText || '',
  //       headers: headers || {},
  //       config: config || item.request,
  //       request: request || null,
  //     };
  //     item._resolve(item.response);
  //     console.log("[response set]", item.response);
  //     // 从队列中移除已完成的请求
  //     this.queue = this.queue.filter((x) => x.id !== requestId);
  //     return this.queue.length; // 返回当前队列长度
  //   }
  //   return -1; // 没有找到对应请求
  // }

  // _enqueueRequest(options) {
  //   const requestId = new Date().getTime() + Math.random().toString(16).slice(2);
  //   const requestItem = {
  //     id: requestId,
  //     request: options,
  //     response: null,
  //   };
  //   console.log("[enqueue request]", options);
  //   this.queue.push(requestItem);

  //   return new Promise((resolve, reject) => {
  //     requestItem._resolve = resolve;
  //     requestItem._reject = reject;
  //     console.log("[request enqueued]", requestItem);
  //   });
  // }
  _request_c(options) {
    return new Promise((resolve, reject) => {
      http.get_request(
        options,
        (res) => {
          try {
            if (!res) {
              reject(new Error("响应为空"));
              return;
            }
            // 2xx 状态码
            if (res.status >= 200 && res.status < 300) {
              resolve(res);
            } else {
              // 非 2xx：reject 一个 Error，把响应对象挂载到 response 属性
              const axiosLikeError = new Error(
                `Request failed with status code ${res.status}`
              );
              axiosLikeError.response = res;
              axiosLikeError.config = res.config; // 补充 config（对齐 axios）
              reject(axiosLikeError);
            }
          } catch (err) {
            reject(err);
          }
        },
        true  // true为开启异步请求
      );
    });
  }

  request(config) {
    return this._request_c(config);
  }

  get(url, config = {}) {
    return this._request_c({ ...config, method: "get", url });
  }

  delete(url, config = {}) {
    return this._request_c({ ...config, method: "delete", url });
  }

  head(url, config = {}) {
    return this._request_c({ ...config, method: "head", url });
  }

  options(url, config = {}) {
    return this._request_c({ ...config, method: "options", url });
  }

  post(url, data = {}, config = {}) {
    return this._request_c({ ...config, method: "post", url, data });
  }

  put(url, data = {}, config = {}) {
    return this._request_c({ ...config, method: "put", url, data });
  }

  patch(url, data = {}, config = {}) {
    return this._request_c({ ...config, method: "patch", url, data });
  }
}

const hijack = new AxiosHijack();

function axios(config) {
  return hijack.request(config);
}

// 复制方法
["get", "delete", "head", "options", "post", "put", "patch"].forEach(
  (method) => {
    axios[method] = hijack[method].bind(hijack);
  }
);

// // 允许外部设置响应
// axios.setResponse = hijack.setResponse.bind(hijack);
// // 允许访问队列
// axios._queue = hijack.queue;

module.exports = axios;
