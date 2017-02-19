local ffi = require('ffi')
local ffi_new = ffi.new
local ffi_cast = ffi.cast
local ffi_string = ffi.string


ffi.cdef[[
    size_t
    jsdecode(char *dst, const char *src,  size_t src_len);
]]


local _M = {
    _VERSION = "0.1.0"
}
local ok, tab_new = pcall(require, "table.new")
if not ok then
    tab_new = function (narr, nrec) return {} end
end


local function find_shared_obj(cpath, so_name)
    local string_gmatch = string.gmatch
    local string_match = string.match
    local io_open = io.open

    for k in string_gmatch(cpath, "[^;]+") do
        local so_path = string_match(k, "(.*/)")
        so_path = so_path .. so_name

        local f = io_open(so_path)
        if f ~= nil then
            io.close(f)
            return so_path
        end
    end
end


local setmetatable = setmetatable
local mt = { __index = _M }
local libjsdecode


function _M.new()
    if libjsdecode == nil then
        local so_path = find_shared_obj(package.cpath, "libjsdecode.so")
        if so_path ~= nil then
            libjsdecode = ffi.load(so_path)
        end
    end

    if libjsdecode == nil then
        return nil, "fail to load libjsdecode.so"
    end

    return setmetatable({}, mt)
end


function _M.decode(entities)
    local buf = ffi_new("char[?]", #entities)
    local size = libjsdecode.jsdecode(buf, entities, #entities)

    return ffi_string(buf, size)
end


return _M
