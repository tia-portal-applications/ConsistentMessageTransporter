
class ServerError extends Error {
  constructor(status, code, msg) {
    super(msg);
    this.status = status;
    this.code = code;
    this.msg = msg;
    this.name = 'ServerError';
  }
  static success() {
    return new ServerError('true', 0, '');
  }
  static failed(code, msg) {
    return new ServerError('false', code, msg);
  }
}


exports = module.exports = ServerError;
