load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "avl_library",
    srcs = ["src/avl.h"],
)

cc_test(
    name = "avl_test",
    size = "small",
    srcs = ["test/avl_test.cpp"],
    deps = [
        ":avl_library",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
