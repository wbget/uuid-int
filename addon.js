const Generator = require('bindings')('addon.node');

module.exports = (id, seed = 0) => {
  if (typeof id !== 'number') {
    throw Error('id need be number');
  }
  if (typeof seed !== 'number') {
    throw Error('seed need be number');
  }
  id = parseInt(id);
  seed = parseInt(seed);
  if (id < 0 || id > 511) {
    throw Error('d must be >=0 or <= 511');
  }
  if (seed < 0 || seed * 1000 > Date.now()) {
    throw Error('seed must <= now');
  }
  const gen = Generator(id, seed);
  gen.id = id;
  gen.seed = seed;
  return gen;
};
