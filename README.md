# uuid-int

uuid-int for nodejs . 

[中文](./README.zh-CN.md)

## Quick Start

Install

```
npm install --save uuid-int
```

Use

```
const UUID = require('uuid-int');

// number  0 <= id <=511
const id = 0;

const generator = UUID(id);

const uuid = generator.uuid();
console.log(uuid); // 3270411116609537
```
