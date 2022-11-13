local TARGET_NAME = "luatos"
local LIB_DIR = "$(buildir)/".. TARGET_NAME .. "/"
local LIB_NAME = "lib" .. TARGET_NAME .. ".a "

local LUATOS_ROOT = SDK_TOP .. "/../LuatOS/"

target(TARGET_NAME)
    set_kind("static")
    set_targetdir(LIB_DIR)
    
    --加入代码和头文件
    add_includedirs("/inc",{public = true})
    add_includedirs(SDK_TOP .. "/interface/private_include", 
                    {public = true})
    add_files("/src/*.c",{public = true})
    add_files(SDK_TOP .. "interface/src/*.c",{public = true})
	
    add_includedirs(LUATOS_ROOT .. "lua/include")
    add_includedirs(LUATOS_ROOT .. "luat/include")
    add_includedirs(LUATOS_ROOT .. "components/cmux")
    add_includedirs(LUATOS_ROOT .. "components/cjson")
    add_includedirs(LUATOS_ROOT .. "components/fatfs")
    add_includedirs(LUATOS_ROOT .. "components/shell")


    add_files(LUATOS_ROOT .. "lua/src/*.c")
    add_files(LUATOS_ROOT .. "luat/modules/*.c")
    add_files(LUATOS_ROOT .. "luat/vfs/*.c")
    add_files(LUATOS_ROOT .. "components/cmux/*.c")
    add_files(LUATOS_ROOT .. "components/cjson/*.c")
    add_files(LUATOS_ROOT .. "components/crypto/*.c")

    -- coremark
    add_files(LUATOS_ROOT .. "components/coremark/*.c")
    add_includedirs(LUATOS_ROOT .. "components/coremark")

    -- lua-cjson
    add_files(LUATOS_ROOT .. "components/lua-cjson/*.c")
    add_includedirs(LUATOS_ROOT .. "components/lua-cjson")

    -- miniz
    add_files(LUATOS_ROOT .. "components/miniz/*.c")
    add_includedirs(LUATOS_ROOT .. "components/miniz")

    -- protobuf
    add_includedirs(LUATOS_ROOT.."components/serialization/protobuf")
    add_files(LUATOS_ROOT.."components/serialization/protobuf/*.c")

    -- fdb
    add_includedirs(SDK_TOP.."thirdparty/fal/inc")
    add_includedirs(SDK_TOP.."thirdparty/flashdb/inc")
    add_files(SDK_TOP.."thirdparty/fal/src/*.c")
    add_files(SDK_TOP.."thirdparty/flashdb/src/*.c")
    add_files(LUATOS_ROOT.."components/flashdb/src/luat_lib_fdb.c")

    -- rsa
    add_files(LUATOS_ROOT.."components/rsa/**.c")

    -- iotauth
    -- add_includedirs(LUATOS_ROOT.."components/iotauth")
    add_files(LUATOS_ROOT.."components/iotauth/**.c")

    -- sfud
    add_includedirs(LUATOS_ROOT.."components/sfud")
    add_files(LUATOS_ROOT.."components/sfud/**.c")

    -- fatfs
    add_includedirs(LUATOS_ROOT.."components/fatfs")
    add_files(LUATOS_ROOT.."components/fatfs/**.c")

    --------------------------------------------------------------
    -- UI类的,能不能用另说, 先加上吧
    
    add_includedirs(LUATOS_ROOT.."lua/include")
    add_includedirs(LUATOS_ROOT.."luat/include")
    add_includedirs(LUATOS_ROOT.."components/lcd")
    add_includedirs(LUATOS_ROOT.."components/tjpgd")
    add_includedirs(LUATOS_ROOT.."components/eink")
    add_includedirs(LUATOS_ROOT.."components/epaper")
    add_includedirs(LUATOS_ROOT.."components/u8g2")
    add_includedirs(LUATOS_ROOT.."components/gtfont")
    add_includedirs(LUATOS_ROOT.."components/qrcode")

    add_includedirs(LUATOS_ROOT.."components/lvgl")
    add_includedirs(LUATOS_ROOT.."components/lvgl/binding")
    add_includedirs(LUATOS_ROOT.."components/lvgl/gen")
    add_includedirs(LUATOS_ROOT.."components/lvgl/src")
    add_includedirs(LUATOS_ROOT.."components/lvgl/font")
    add_includedirs(LUATOS_ROOT.."components/lvgl/src/lv_font")

    -- lvgl
    add_files(LUATOS_ROOT.."components/lvgl/**.c")

    -- eink
    add_files(LUATOS_ROOT.."components/eink/*.c")
    add_files(LUATOS_ROOT.."components/epaper/*.c")
    remove_files(LUATOS_ROOT.."components/epaper/GUI_Paint.c")

    -- u8g2
    add_files(LUATOS_ROOT.."components/u8g2/*.c")

    -- lcd
    add_files(LUATOS_ROOT.."components/lcd/*.c")
    add_files(LUATOS_ROOT.."components/tjpgd/*.c")
    add_files(LUATOS_ROOT.."components/qrcode/*.c")
    

    -- network
    add_includedirs(LUATOS_ROOT .. "components/ethernet/common")
    add_includedirs(LUATOS_ROOT .. "components/common")
    add_includedirs(LUATOS_ROOT .. "components/network/adapter")
    add_includedirs(LUATOS_ROOT .. "components/mbedtls/include", {public = true})
    add_files(LUATOS_ROOT .. "components/ethernet/common/*.c")
    add_files(LUATOS_ROOT .. "components/network/adapter/*.c")
    add_files(LUATOS_ROOT .. "components/mbedtls/library/*.c")

    -- mqtt
    add_includedirs(LUATOS_ROOT.."components/network/libemqtt")
    add_files(LUATOS_ROOT.."components/network/libemqtt/*.c")

    -- http
    add_includedirs(LUATOS_ROOT.."components/network/libhttp")
    add_files(LUATOS_ROOT.."components/network/libhttp/luat_lib_http.c")

    -- iotauth
    add_files(LUATOS_ROOT.."components/iotauth/luat_lib_iotauth.c")

    ------------------------------------------------------------
    ------------------------------------------------------------


    -- 作为最后补充, 不然总是报主库没有的头文件
    add_includedirs(SDK_TOP .. "interface/include")

    --自动链接
    LIB_USER = LIB_USER .. SDK_TOP .. LIB_DIR .. LIB_NAME .. " "
    --甚至可以加入自己的库
target_end()