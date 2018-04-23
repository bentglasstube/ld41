git_repository(
    name = "libgam",
    remote = "https://github.com/bentglasstube/gam.git",
    commit = "96f9154af3f846aac073c6389a47880dec71ad7b",
)

git_repository(
    name = "mxebzl",
    remote = "https://github.com/cfrantz/mxebzl.git",
    tag = "20170703_RC03",
)

load("@mxebzl//tools:repository.bzl", "mxe_compilers")
mxe_compilers(
    deps = [
        "compiler",
        "SDL2",
        "SDL2-extras",
        "xz",
    ],
)
