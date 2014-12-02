
/****************************************************************************/
/* DSA list program example D.F. ROSS                                       */
/****************************************************************************/

/****************************************************************************/
/* include files                                                            */
/****************************************************************************/
#include <stdio.h>
#include "uidialog.h"
#include "begraph.h"

/****************************************************************************/
/* front end functions  = backend functions with preconditions              */
/****************************************************************************/

/****************************************************************************/
/* display adjacency list - IF graph not empty                              */
/****************************************************************************/
void fe_display_adjlist()  {
   if (be_size()==0) ui_putGraphEmpty();
   else { ui_putTitleList(); be_display_adjlist(); }
   }

/****************************************************************************/
/* display adjacency matrix - IF graph not empty                            */
/****************************************************************************/
void fe_display_adjmatrix()  {
   if (be_size()==0) ui_putGraphEmpty();
   else { ui_putTitleMatrix(); be_display_adjmatrix(); }
   }

/****************************************************************************/
/* add a node - IF the node does not already exist!  i.e. node not in V     */
/****************************************************************************/
void fe_addnode()  {
   char c;

   c = ui_getNode();
   if (be_is_nmember(c)) ui_putNodeError(c);
   else be_addnode(c);
   }

/****************************************************************************/
/* add an edge (x,y) - IF  graph not empty and x, y in V and (x,y) NOT in E */
/****************************************************************************/
void fe_addedge()  {
 
   char cs, cd; int w;

   if (be_size()==0) ui_putGraphEmpty();
   else {
      cs = ui_getNode();
      if (!be_is_nmember(cs)) ui_putNodeError(cs);
      else {
         cd = ui_getNode();
         if (!be_is_nmember(cd)) ui_putNodeError(cd);
         else if (be_is_emember(cs, cd)) ui_putEdgeError(cs, cd);
         else {
            w  = ui_getWeight();
            be_addedge(cs, cd, w); 
            if (ui_isModeU() && (cd != cs)) be_addedge(cd, cs, w);
            }
         }
      }
   }

/****************************************************************************/
/* remove a node - IF G not empty and node in V                             */
/****************************************************************************/
void fe_remnode()  {

   char c;

   if (be_size()==0) ui_putGraphEmpty();
   else {
      c = ui_getNode();
      if(!be_is_nmember(c)) ui_putNoNodeError(c);
      else be_remnode(c);
      }
   }

/****************************************************************************/
/* remove an edge (x,y) - IF G not empty and x, y in V and (x,y) in E       */
/****************************************************************************/
 void fe_remedge()  {

   char cs, cd;

   if (be_size()==0) ui_putGraphEmpty();
   else {
      cs = ui_getNode();
      if (!be_is_nmember(cs)) ui_putNoNodeError(cs);
      else {
         cd = ui_getNode();
         if (!be_is_nmember(cd)) ui_putNoNodeError(cd);
         else if (!be_is_emember(cs, cd)) ui_putNoEdgeError(cs, cd);
         else {
            be_remedge(cs, cd);
            if (ui_isModeU()) {
              if (!be_is_emember(cd, cs)) ui_putNoEdgeError(cs, cd);
              else be_remedge(cd, cs);
              }
            }
         }
      }
   }

/****************************************************************************/
/* is a node in V - IF G not empty ==> found / not found message            */
/****************************************************************************/
void fe_is_nmember() {
   char c;

   if (be_size()==0) ui_putGraphEmpty();
   else {
      c = ui_getNode();
      if (be_is_nmember(c)) ui_putNodeFound(c);
      else  ui_putNodeNotFound(c);
      }
   }

/****************************************************************************/
/* is an edge (x,y) in E - IF G not empty and x, y in V                     */
/*                   ==> found / not found message                          */
/****************************************************************************/
void fe_is_emember() {
   char cs, cd;

   if (be_size()==0) ui_putGraphEmpty();
   else {
      cs = ui_getNode();
      if (!be_is_nmember(cs)) ui_putNoNodeError(cs);
      else {
         cd = ui_getNode();
         if (!be_is_nmember(cd)) ui_putNoNodeError(cd);
         else if (be_is_emember(cs, cd)) ui_putEdgeFound(cs, cd);
         else ui_putEdgeNotFound(cs, cd);
         }
      }
   }

/****************************************************************************/
/* Graph Algorithms                                                         */
/****************************************************************************/
void fe_Dijkstra() { be_Dijkstra(); }
void fe_Floyd()    { be_Floyd();    }
void fe_Warshall() { be_Warshall(); }
void fe_Prim()     { be_Prim();     }

/****************************************************************************/
/* end of front end functions                                               */
/****************************************************************************/



