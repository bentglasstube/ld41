package(default_visibility = ["//visibility:public"])

load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")
load("@mxebzl//tools/windows:rules.bzl", "pkg_winzip")

config_setting(
    name = "windows",
    values = {
        "crosstool_top": "@mxebzl//tools/windows:toolchain",
    }
)

cc_binary(
    name = "ld41",
    data = ["//content"],
    linkopts = select({
        ":windows": ["-mwindows", "-lSDL2main" ],
        "//conditions:default": [],
    }) + [
        "-lSDL2",
        "-lSDL2_image",
        "-lSDL2_mixer",
        "-static-libstdc++",
        "-static-libgcc",
    ],
    srcs = ["main.cc"],
    deps = [
        "@libgam//:game",
        ":screens",
    ],
)

pkg_winzip(
    name = "ld41-windows",
    files = [
        ":ld41",
        "//content",
    ]
)

pkg_tar(
    name = "ld41-linux",
    extension = "tar.gz",
    srcs = [
        ":ld41",
        "//content",
    ],
)

cc_library(
    name = "bar",
    srcs = ["bar.cc"],
    hdrs = ["bar.h"],
    deps = [
        "@libgam//:graphics",
        "@libgam//:spritemap",
    ],
)

cc_library(
    name = "objects",
    srcs = [
        "bullet.cc",
        "enemy.cc",
        "explosion.cc",
        "object.cc",
        "powerup.cc",
        "ship.cc",
    ],
    hdrs = [
        "bullet.h",
        "enemy.h",
        "explosion.h",
        "object.h",
        "powerup.h",
        "ship.h",
    ],
    deps = [
        "@libgam//:audio",
        "@libgam//:graphics",
        "@libgam//:spritemap",
        "@libgam//:util",
    ],
)

cc_library(
    name = "puzzle",
    srcs = ["puzzle.cc"],
    hdrs = ["puzzle.h"],
    deps = [
        "@libgam//:graphics",
        "@libgam//:parallax_backdrop",
        "@libgam//:spritemap",
        "@libgam//:util",
    ],
)

cc_library(
    name = "screens",
    srcs = [
        "puzzle_screen.cc",
        "title_screen.cc",
    ],
    hdrs = [
        "puzzle_screen.h",
        "title_screen.h",
    ],
    deps = [
        ":bar",
        ":objects",
        ":puzzle",
        "@libgam//:backdrop",
        "@libgam//:parallax_backdrop",
        "@libgam//:screen",
        "@libgam//:spritemap",
        "@libgam//:text",
    ],
)
