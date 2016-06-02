#!/usr/bin/python
# -*- coding: utf-8 -*-
def foo(n):
    count = 1
    if n > 0:
        count += foo(n - 1) + foo(n - 1)
    
    print '1'
    return count
    
print foo(1)

print "hello"
