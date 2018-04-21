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
        ":title_screen",
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
    name = "puzzle",
    srcs = ["puzzle.cc"],
    hdrs = ["puzzle.h"],
    deps = [
        "@libgam//:graphics",
        "@libgam//:spritemap",
        "@libgam//:util",
    ],
)

cc_library(
    name = "puzzle_screen",
    srcs = ["puzzle_screen.cc"],
    hdrs = ["puzzle_screen.h"],
    deps = [
        ":puzzle",
        "@libgam//:screen",
        "@libgam//:spritemap",
        "@libgam//:text",
    ],
)

cc_library(
    name = "title_screen",
    srcs = ["title_screen.cc"],
    hdrs = ["title_screen.h"],
    deps = [
        ":puzzle_screen",
        "@libgam//:screen",
        "@libgam//:text",
    ],
)
