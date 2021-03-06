/**
 * Copyright (c) 2010 - 2011 Distillate Project
 *
 *  ______ ________________________            _____________________
 *  |     \  |  |______   |     |  |     |     |_____|   |   |______
 *  |_____/__|________|   |   __|__|_____|_____|     |   |   |______
 *
 *
 * License: BSD
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Wintermoon nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __GROUP_HPP__
#define __GROUP_HPP__

#include <vector>
#include <string>
#include "Defs.hpp"
#include "Object.hpp"

NAMESPACE_BEGIN

/* Forward */
class Point;

/**
 * This is an organizational class that can update and render a bunch of <code>Object</code>s.
 * NOTE: Although <code>Group</code> extends <code>Object</code>, it will not automatically
 * add itself to the global collisions quad tree, it will only add its members.
 */
class Group : public Object {
     friend class DState;

public:
     typedef enum {
          ASCENDING = -1,
          DESCENDING = 1
     } SortType;

protected:
     /**
      * Array of all the <code>Object</code>s that exist in this layer.
      */
     std::vector<Object*> members;

     /**
      * Helpers for moving/updating group members.
      */
     Point _last;
     bool _first;

     /**
      * Helpers for sorting members
      */
     std::string _sortIndex;
     int _sortOrder;

     /**
      * Internal function, helps with the moving/updating of group members.
      */
     void saveOldPosition();

     /**
      * Internal function that actually goes through and updates all the group members.
      * Depends on <code>saveOldPosition()</code> to set up the correct values in <code>_last</code> in order to work properly.
      */
     void updateMembers();

     /**
      * Internal function that calls kill on all members.
      */
     void killMembers();

     /**
      * Internal function that actually loops through and destroys each member.
      */
     void destroyMembers();

public:
     Group();
     virtual ~Group();

     /**
      * Adds a new <code>Object</code> subclass (DSprite, DBlock, etc) to the list of children
      *
      * @param   Object          The object you want to add
      * @param   ShareScroll     Whether or not this DCore should sync up with this layer's scrollFactor
      *
      * @return  The same <code>DCore</code> object that was passed in.
      */
     Object* add(Object *Obj, bool ShareScroll=false);

     /**
      * Replaces an existing <code>Object</code> with a new one.
      *
      * @param   OldObject   The object you want to replace.
      * @param   NewObject   The new object you want to use instead.
      *
      * @return  The new object.
      */
     Object* replace(Object *OldObject, Object *NewObject);

     /**
      * Removes an object from the group.
      *
      * @param   Object  The <code>Object</code> you want to remove.
      * @param   Splice  Whether the object should be cut from the
      * array entirely or not.
      *
      * @return  The removed object.
      */
     Object* remove(Object *Obj, bool Splice=false);

     /**
      * Call this function to sort the group according to a particular
      * value and order.
      * For example, to sort game objects for Zelda-style overlaps you might call
      * <code>myGroup.sort("y",ASCENDING)</code> at the bottom of your
      * <code>DState.update()</code> override.
      * To sort all existing objects after
      * a big explosion or bomb attack, you might call
      * <code>myGroup.sort("exists",DESCENDING)</code>.
      *
      * @param   Index   The <code>String</code> name of the member
      * variable you want to sort on.  Default value is "y".
      * @param   Order   A <code>Group</code> constant that defines
      * the sort order.  Possible values are <code>ASCENDING</code>
      * and <code>DESCENDING</code>.  Default value is <code>ASCENDING</code>.
      */
     void sort(SortType Order=ASCENDING);

     /**
      * Call this function to retrieve the first object with exists == false in the group.
      * This is handy for recycling in general, e.g. respawning enemies.
      *
      * @return  A <code>Object</code> currently flagged as not existing.
      */
     Object* getFirstAvail();

     /**
      * Call this function to retrieve the first index set to 'null'.
      * Returns -1 if no index stores a null object.
      *
      * @return  An <code>int</code> indicating the first null slot in the group.
      */
     int getFirstNull();

     /**
      * Finds the first object with exists == false and calls reset on it.
      *
      * @param   X   The new X position of this object.
      * @param   Y   The new Y position of this object.
      *
      * @return  Whether a suitable <code>Object</code> was found and reset.
      */
     bool resetFirstAvail(float X=0, float Y=0);

     /**
      * Call this function to retrieve the first object with exists == true in the group.
      * This is handy for checking if everything's wiped out, or choosing a squad leader, etc.
      *
      * @return  A <code>Object</code> currently flagged as existing.
      */
     Object* getFirstExtant();

     /**
      * Call this function to retrieve the first object with dead == false in the group.
      * This is handy for checking if everything's wiped out, or choosing a squad leader, etc.
      *
      * @return  A <code>Object</code> currently flagged as not dead.
      */
     Object* getFirstAlive();

     /**
      * Call this function to retrieve the first object with dead == true in the group.
      * This is handy for checking if everything's wiped out, or choosing a squad leader, etc.
      *
      * @return  A <code>Object</code> currently flagged as dead.
      */
     Object* getFirstDead();

     /**
      * Call this function to find out how many members of the group are not dead.
      *
      * @return  The number of <code>Object</code>s flagged as not dead.  Returns -1 if group is empty.
      */
     int countLiving();

     /**
      * Call this function to find out how many members of the group are dead.
      *
      * @return  The number of <code>Object</code>s flagged as dead.  Returns -1 if group is empty.
      */
     int countDead();

     /**
      * Returns a count of how many objects in this group are on-screen right now.
      *
      * @return  The number of <code>Object</code>s that are on screen.  Returns -1 if group is empty.
      */
     int countOnScreen();

     /*
      * Count
      */
     int count() {
          return members.size();
     };

     /**
      * Returns a member at random from the group.
      *
      * @return  A <code>Object</code> from the members list.
      */
     Object* getRandom();

     /**
      * Automatically goes through and calls update on everything you added,
      * override this function to handle custom input and perform collisions.
      */
     void update();

     /**
      * Internal function that actually loops through and renders all the group members.
      */
     void renderMembers();

     /**
      * Automatically goes through and calls render on everything you added,
      * override this loop to control render order manually.
      */
     void render();

     /**
      * Calls kill on the group and all its members.
      */
     void kill();

     /**
      * Override this function to handle any deleting or "shutdown" type operations you might need,
      * such as removing traditional Flash children like Sprite objects.
      */
     void destroy();

     /**
      * If the group's position is reset, we want to reset all its members too.
      *
      * @param   X   The new X position of this object.
      * @param   Y   The new Y position of this object.
      */
     void reset(float X, float Y);

     int sortHandler(Object* Obj1, Object* Obj2);
};

NAMESPACE_END    
#endif /* __GROUP_HPP__ */
