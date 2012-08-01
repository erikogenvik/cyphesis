#This file is distributed under the terms of the GNU General Public license.
#Copyright (C) 1999 Aloril (See the file COPYING for details).

from types import *
from atlas import *
from common import log

## \defgroup PythonGoals Goal Classes

## \brief Base class for all goals
## \ingroup PythonGoals
class Goal:
    def __init__(self,desc="some goal",fulfilled=None,subgoals=[],
                 time=None, debug=0):
        self.desc=desc
        #mind sets these:
        #self.str
        #self.key
        if fulfilled: self.fulfilled=fulfilled
        else: self.fulfilled=lambda me:0 #false
        self.subgoals=subgoals[:]
        self.time=time
        self.debug=debug
        self.vars=[]
        #keeps track of whether the goal is fulfilled or not
        #this is mainly of use for inspection and diagnosis
        self.is_fulfilled=0
        #any subgoal/function/method can set this and 
        #it's checked at start of check_goal_rec
        #and NPCMind.py fulfill_goals uses it too to remove goals from list
        self.irrelevant=0
    def __repr__(self):
        return self.info()
    def info(self):
        name=self.__class__.__name__
        if name=="Goal":
            return name+"("+`self.desc`+")"
        var=""
        for v in self.vars:
            if var: var=var+","
            var=var+`getattr(self,v)`
        return name+"("+var+")"
    
    def extendedInfo(self, indent=1):
        """provides extended information about the goal,
        as well as all subgoals"""
        name=self.__class__.__name__
        if name=="Goal":
            res = name+"("+`self.desc`+") : "
            if (self.is_fulfilled):
                res = res + 'fulfilled'
            else:
                res = res + 'not fulfilled'
            for sg in self.subgoals:
                if type(sg)!=FunctionType and type(sg)!=MethodType:
                    res = res + "\n" + ("\t" * indent) + sg.extendedInfo(indent+1)
            return res
        var=""
        for v in self.vars:
            if var: var=var+","
            var=var+`getattr(self,v)`
        res = name+"("+var+") : "
        if (self.is_fulfilled):
            res = res + 'fulfilled'
        else:
            res = res + 'not fulfilled'
        for sg in self.subgoals:
            if type(sg)!=FunctionType and type(sg)!=MethodType:
                res = res + "\n" + ("\t" * indent) + sg.extendedInfo(indent+1)
        return res
    def check_goal(self, me, time):
        "executes goal, see top of file"
        if self.debug:
            log.thinking("GOAL desc: "+self.str)
        res,deb=self.check_goal_rec(me,time,0)
        if len(deb)!=0:
            deb=self.info()+"."+deb
            # print deb
        if res!=None:
            info_ent=Entity(op=res,description=deb)
            return res+Operation("goal_info",info_ent)
    def check_goal_rec(self, me, time, depth):
        """check (sub)goal recursively"""
        res,deb=None,""
        if self.irrelevant: return res,deb
        #is it right time range?
        if self.time and not time.is_now(self.time): return res,deb
        if self.debug:
            log.thinking("\t"*depth+"GOAL: bef fulfilled: "+self.desc+" "+`self.fulfilled`)
        if self.fulfilled(me): 
            self.is_fulfilled = 1
            return res,deb
        else:
            self.is_fulfilled = 0
        for sg in self.subgoals:
            if type(sg)==FunctionType or type(sg)==MethodType:
                res=sg(me)
                if res==None:
                    if self.debug:
                        log.thinking("\t"*depth+"GOAL: function: "+`sg`+" "+`res`)
                deb=sg.__name__+"()"
                return res,deb
            else:
                if self.debug:
                    log.thinking("\t"*depth+"GOAL: bef sg: "+sg.desc)
                res,deb=sg.check_goal_rec(me,time,depth+1)
                if self.debug: 
                    log.thinking("\t"*depth+"GOAL: aft sg: "+sg.desc+" "+str(res))
                if len(deb)>0:
                    deb=sg.info()+"."+deb
                    return res,deb
        return res,deb
