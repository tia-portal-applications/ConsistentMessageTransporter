const log4js = require('log4js');
log4js.configure({
  appenders: {
    Console: { type: 'stdout', layout: { type: 'messagePassThrough'}},
    VerboseLog: {
      type: 'file', filename: './log/verbose.log', filesToKeep: 1, keepFileExt: true, maxLogSize: 1024 * 500, backups: 2, compress: true,
      layout: {
        type: 'pattern',
        pattern: '[%d] [%p]:%m%n'
      }
    },
    InfoLog: {
      type: 'dateFile', filename: './log/info.log', filesToKeep: 1, keepFileExt: true, maxLogSize: 1024 * 500, backups: 2, compress: true,
      layout: {
        type: 'pattern',
        pattern: '[%d] [%p]:%m%n'
      }
    },
    ErrorLog: {
      type: 'file', filename: './log/error.log', filesToKeep: 1, keepFileExt: true, maxLogSize: 1024 * 500, backups: 2, compress: true,
      layout: {
        type: 'pattern',
        pattern: '[%d] [%p]:%m%n'
      }
    }
  },
  categories: {
    default: { appenders: ['Console', 'InfoLog', 'VerboseLog'], level: 'info' },
    Error: { appenders: ['Console', 'ErrorLog', 'VerboseLog'], level: 'error' }
  }

});

exports = module.exports = log4js;


