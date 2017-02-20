# lua-resty-jsdecode
[Javascript Escape Notation Decode](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String) To UTF-8 Bytes with the Luajit ffi binding

Table of Contents
-----------------
* [Status](#status)
* [Compile](#compile)
* [Usage](#usage)
* [Todo](#todo)
* [Contributing](#contributing)
* [Author](#author)
* [License](#license)

Status
====
[![Build Status](https://travis-ci.org/detailyang/lua-resty-jsdecode.svg?branch=master)](https://travis-ci.org/detailyang/lua-resty-jsdecode)

Production Ready

Compile
=======

```bash
make;
```

Usage
====
Make sure `jsdecode.lua` and `libjsdecode.so` is in the `package.cpath` and `package.path`

```bash
local jsdecoder = jsdecode.new()

jsdecoder.decode("\\u0062\\u006f\\u006f\\u006d") #boom
```

Todo
=====
* Hexadecimal escape sequences
* Latin-1 character

Contributing
------------

To contribute to lua-resty-jsdecode, clone this repo locally and commit your code on a separate branch.

PS: PR Welcome :rocket: :rocket: :rocket: :rocket:


Author
------

> GitHub [@detailyang](https://github.com/detailyang)


License
-------
lua-resty-jsdecode is licensed under the [MIT] license.

[MIT]: https://github.com/detailyang/ybw/blob/master/licenses/MIT
