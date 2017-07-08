#!/bin/bash

doxygen Doxyfile
rm -r ../docs/*
cp -r ./html/* ../docs
