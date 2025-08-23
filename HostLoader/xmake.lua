add_rules("mode.debug", "mode.release")

add_repositories("groupmountain-repo https://github.com/GroupMountain/xmake-repo.git")
add_repositories("xmake-repo https://github.com/xmake-io/xmake-repo.git")

add_requires("endstone 0.9.1")
add_requires("dotnet 9.0.3")

if is_plat("windows") and not has_config("vs_runtime") then
    set_runtimes("MD")
end

if is_plat("linux") then
    set_toolchains("clang")
end

target("my-plugin")
    set_kind("binary")
    set_languages("c++23")
    add_packages(
        "endstone",
        "dotnet"
    )
    add_includedirs("src")
    add_files("src/**.cpp")

    if is_plat("windows") then
        add_defines("NOMINMAX")
        add_cxflags(
            "/EHsc",  
            "/utf-8", 
            "/W4"     
        )
        add_links("nethost")
    else
        add_cxxflags("-Wno-gnu-line-marker")
        add_cxflags(
            "-Wall",
            "-pedantic",
            "-fexceptions",
            "-stdlib=libc++"
        )
        add_ldflags(
            "-stdlib=libc++"
        )
        add_links("nethost")
    end

    after_build(function(target)
        local file = target:targetfile()
        local output_dir = path.join(os.projectdir(), "bin")
        os.mkdir(output_dir)
        local filename = path.filename(file)

        if os.host() == "linux" then
            filename = filename:sub(4)
        end

        os.cp(file, path.join(output_dir, filename))

        if os.host() == "windows" then
            local nethost_path = "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/9.0.8/runtimes/win-x64/native/nethost.dll"
            if os.exists(nethost_path) then
                os.cp(nethost_path, path.join(output_dir, "nethost.dll"))
                cprint("${bright green}[Plugin]: ${reset}nethost.dll copied to " .. output_dir)
            else
                cprint("${bright red}[Plugin]: ${reset}Warning: nethost.dll not found at " .. nethost_path)
            end
        end

        cprint("${bright green}[Plugin]: ${reset} generated into " .. output_dir)
    end)