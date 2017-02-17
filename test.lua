package.cpath = package.cpath..";./?.so"
package.path = package.cpath..";./?.lua"


local jsdecode = require("jsdecode")

local decoder, err = jsdecode.new()
if err then
    assert(false, "cannot found libjsdecode.so")
end

local function expect(src, dst)
    assert(decoder.decode(src), dst, "fail to pass " .. src)
    print(string.format("test success: %s => %s", src, dst))
end


expect("\\u0026\\u006c\\u0074\\u003b\\u0069\\u006d\\u0067\\u0020\\u0073\\u0072\\u0063\\u003d\\u0026\\u0071\\u0075\\u006f\\u0074\\u003b\\u0078\\u0078\\u0026\\u0071\\u0075\\u006f\\u0074\\u003b\\u0020\\u002f\\u0026\\u0067\\u0074\\u003b", "&lt;img src=&quot;xx&quot; /&gt")