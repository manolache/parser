#!/bin/bash
if ./out/parser res/empty.ini res/first.ini res/update.ini; then
       echo "SUCCESS"
else
       echo "FAILED"
fi