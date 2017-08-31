#!/bin/bash
for i in $(echo $@ | tr ',' ' '); do echo -n "$((79 - $i)) "; done; echo
	     
