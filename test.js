const assert = require('assert');
const UUID = require('./addon');

assert.throws(() => UUID(), Error);
assert.throws(() => UUID(-1), Error);
assert.throws(() => UUID(512), Error);
assert.throws(() => UUID('28'), Error);

const t1 = UUID(1);
const t2 = UUID(2);
assert(t1.id === 1);
assert(t2.id === 2);
assert(((t1.uuid() !== t1.uuid()) !== t2.uuid()) !== t2.uuid());

for (let i = 0; i < 1000; i++) {
  assert(t1.uuid() !== t2.uuid());
}
const map = {};
const t3 = UUID(3);
for (let i = 0; i < 4096; i++) {
  const uuid = t3.uuid();
  if (map[uuid]) {
    assert(false);
  } else {
    map[uuid] = 1;
  }
}

const t4 = UUID(4);
let catchError = false;
for (let i = 0; i < 10000; i++) {
  try {
    t4.uuid();
  } catch (error) {
    catchError = true;
  }
}
assert(catchError);
console.log(t1.uuid());
