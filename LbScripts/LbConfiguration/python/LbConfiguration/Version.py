""" LHCb version style definition and massaging """
# $Id: Version.py,v 1.2 2009-12-15 17:11:38 hmdegaud Exp $

import re


# This is the version style used in the releases: vXrY[pZ]
_txt_version_style = r'v([0-9]+)r([0-9]+)(?:p([0-9]+))?'
version_style = re.compile(_txt_version_style)

# This is the version style for the LCGCMT releases: 56[a]
_txt_lcg_version_style = r'([0-9]+)([a-z]?)'
lcg_version_style  = re.compile(_txt_lcg_version_style)

# user version style : hmd_091008
_txt_user_cvs_version_style = r'(?:\w+_([0-9]{4})([0-9]{2})([0-9]{2})[a-z]?)'
user_cvs_version_style = re.compile(_txt_user_cvs_version_style)

# full version style used in CVS: the regular style with the possible "pre" suffix or the user ones.
_txt_cvs_version_style = r'%s(?:-pre([0-9]+))?|%s'% (_txt_version_style, _txt_user_cvs_version_style)

cvs_version_style = re.compile(_txt_cvs_version_style)

class NotAVersion(Exception):
    pass

class CoreVersion:
    version_style = re.compile("%s$" % _txt_version_style)
    def __init__(self, vname):
        self._vname = vname
        self._version = None
        self._patchversion = False
        m = self.version_style.match(self._vname)
        if m :
            a, b, c = m.groups()
            if a is None or b is None :
                raise NotAVersion
            a = int(a)
            b = int(b)
            if c is not None:
                self._patchversion = True
                c = int(c)
            self._version = (a, b, c)
        else :
            raise NotAVersion
    def __str__(self):
        return self._vname
    def __repr__(self):
        return "%s %s" % (self.__class__, self._vname)
    def __cmp__(self, other):
        return cmp(self._version, other.version())
    def name(self):
        return self._vname
    def version(self):
        return self._version


def sortVersions(versionlist, versiontype=CoreVersion, safe=False):
    if not safe :
        vlist = [ versiontype(x) for x in versionlist ]
    else :
        vlist = []
        for v in versionlist :
            try :
                v = versiontype(v)
            except NotAVersion :
                continue
            vlist.append(v)
    vlist.sort()
    sorted_list = [ x.name() for x in vlist]
    return sorted_list
