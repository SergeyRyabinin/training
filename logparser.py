#!/usr/bin/env python
# -*- coding: utf-8 -*-

import re
import datetime

def check_int(s):
    if s[0] in ('-', '+'):
        return s[1:].isdigit()
    return s.isdigit()

def parse_field(value):
    if(value == "-"):
        return None
    if(check_int(value)):
        return int(value)
    else:
        return value
    #return None if value == '-' else int(value)

class LogEntry(object):
    def __init__(self, **kwargs):
        for key, value in kwargs.items():
            #print("%s = %s" % (key, value))
            setattr(self, key, parse_field(value))
        #08/Sep/2013:06:29:02 +0200
        #if hasattr(self, "datetime"):
        #    datetime_object = datetime.strptime(self.datetime, '%D/%M/%Y:%H:%M:%S')

    def Old__init__(self, logLine):
        self.logLine = logLine
        
        logItems = self.logLine.split()
        self.ip = logItems[0]
        self.user = logItems[1]
        self.clientid = logItems[2]
        self.datetime = logItems[3]+" "+logItems[4]
        self.verb = logItems[5][1:]
        self.accessPath = logItems[6]
        self.protocol = logItems[7][:-1]
        self.response_code = self._parse_int(logItems[8])
        self.rsSize = self._parse_int(logItems[9])
        self.referrer = logItems[10]
        self.rqSize = self._parse_int(logItems[-1])
        
    def match(self, **kwargs):
        #import pdb
        #pdb.set_trace()
        for key, value in kwargs.items():
            print("%s = %s" % (key, value))
            if not hasattr(self, key):
                return False
            if not getattr(self, key, lambda: None) == value:
                return False
        return True
        
    def __str__(self):
        s = ""
        #for attr, val in self.__dict__.iteritems():
        for attr, val in vars(self).items():
            s += '%s: %s\n' % (attr, val)
        return s
        
    def _parse_int(self, value):
        #return int(value) if value.isdigit() else None
        return None if value == '-' else int(value)

PATTERN = re.compile(
    "^"
    "(?P<ip>\d+\.\d+\.\d+\.\d+) " #IP
    "(?P<user>[-\w]+) "           #user -
    "(?P<clientid>[-\w]+) "       #clientid -
    "\[(?P<datetime>.*)\] "       #datetime [08/Sep/2013:06:29:02 +0200]
    "\"(?P<verb>.*) /.*\" "       #verb "GET
    "(?P<response_code>\d+).* "   #response_code 302
    ".*$"
)
def parse_alog_line(line):
    m = PATTERN.match(line)
    if m is not None:
        return LogEntry(**m.groupdict())

if __name__ == "__main__":
    line = """18.1.1.201 - - [08/Sep/2013:06:29:02 +0200] "GET / HTTP/1.1" 302 312 "-" "check_http/v1.4.15 (nagios-plugins 1.4.15)" 530"""
    print(parse_alog_line(line))

    #with open('../input/access.log') as f:
    #    for line in f: # -> f.xreadlines()
    #        log = LogEntry( line )
    #        print(log)


