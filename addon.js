var Generator = require('bindings')('addon.node');

module.exports = (id, seed = 0) => Generator(id, seed);
