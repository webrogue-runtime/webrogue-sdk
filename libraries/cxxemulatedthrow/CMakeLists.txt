cmake_minimum_required(VERSION 3.31)

project(cxxemulatedthrow)

add_library(cxxemulatedthrow STATIC cxxemulatedthrow.c)

install(TARGETS cxxemulatedthrow)
