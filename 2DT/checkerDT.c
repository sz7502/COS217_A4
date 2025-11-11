/*--------------------------------------------------------------------*/
/* checkerDT.c                                                        */
/* Author:                                                            */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "checkerDT.h"
#include "dynarray.h"
#include "path.h"



/* see checkerDT.h for specification */
boolean CheckerDT_Node_isValid(Node_T oNNode) {
   Node_T oNParent;
   Path_T oPNPath;
   Path_T oPPPath;

   /* Sample check: a NULL pointer is not a valid node */
   if(oNNode == NULL) {
      fprintf(stderr, "A node is a NULL pointer\n");
      return FALSE;
   }

   /* Sample check: parent's path must be the longest possible
      proper prefix of the node's path */
   oNParent = Node_getParent(oNNode);
   if(oNParent != NULL) {
      oPNPath = Node_getPath(oNNode);
      oPPPath = Node_getPath(oNParent);

      if(Path_getSharedPrefixDepth(oPNPath, oPPPath) !=
         Path_getDepth(oPNPath) - 1) {
         fprintf(stderr, "P-C nodes don't have P-C paths: (%s) (%s)\n",
                 Path_getPathname(oPPPath), Path_getPathname(oPNPath));
         return FALSE;
      }
   }

   return TRUE;
}

/* ASK IF THIS IS OK */

/* Ensures that all children of oNNode do not share identical paths and
   that they are in order. If they do not meet the criteria, return
   FALSE. Otherwise, return TRUE. */
static boolean CheckerDT_hasValidChildren(Node_T oNNode) {
   size_t ulNumChildren = Node_getNumChildren(oNNode);
   size_t ulIndex;
   size_t ulIndex2;
   
   for(ulIndex = 0; ulIndex < ulNumChildren; ulIndex++)
   {
      for (ulIndex2 = (ulIndex + 1); ulIndex2 < ulNumChildren; ulIndex2++) 
      {
         Node_T oNChild = NULL;
         Node_T oNChild2 = NULL;
         int iStatus = Node_getChild(oNNode, ulIndex, &oNChild);
         const char* oNChildStringPath;
         const char* oNChild2StringPath;

         if(iStatus != SUCCESS) {
            continue;
         }

         iStatus = Node_getChild(oNNode, ulIndex2, &oNChild2);

         if(iStatus != SUCCESS) {
            continue;
         }

         /* To be in in proper order, oNChild must always be less than
            oNChild2 */
         
         oNChildStringPath = Path_getPathname(Node_getPath(oNChild));
         oNChild2StringPath = Path_getPathname(Node_getPath(oNChild2));

         if (strcmp(oNChildStringPath, oNChild2StringPath) >= 0) {
            return FALSE;
         }
      }
   }
   return TRUE;
}

/*
   Performs a pre-order traversal of the tree rooted at oNNode.
   Returns FALSE if a broken invariant is found and
   returns TRUE otherwise.

   You may want to change this function's return type or
   parameter list to facilitate constructing your checks.
   If you do, you should update this function comment.
*/
/* UPDATEUPDATEUPDATEUPDATEUPDATEUPDATEUPDATEUPDATEUPDATEUPDATEUPDATEUPDATEUPDATEUPDATEUPDATEUPDATEUPDATEUPDATE */
static int CheckerDT_treeCheck(Node_T oNNode, int iNodeCount) {
   size_t ulIndex;

   if(oNNode!= NULL) {
      Node_T parent = Node_getParent(oNNode);
      iNodeCount++;
      /*
      if (parent) {
         printf("%s: children-%u, parent-%s\n", Path_getPathname(Node_getPath(oNNode)), Node_getNumChildren(oNNode), Path_getPathname(Node_getPath(parent)));
      } else {
         printf("%s: children-%u, parent-%s\n", Path_getPathname(Node_getPath(oNNode)), Node_getNumChildren(oNNode), "Orphan");
      }

      printf("NodeCount: %i\n", iNodeCount);
      printf("Running looper\n");

      for(ulIndex = 0; ulIndex < Node_getNumChildren(oNNode); ulIndex++)
      {
         Node_T oNChild = NULL;
         int iStatus = Node_getChild(oNNode, ulIndex, &oNChild);
         if(iStatus != SUCCESS) {
            printf("Looper - getNumChildren claims more children than getChild returns\n");
         } else {
            printf("Looper Child: %s\n", Path_getPathname(Node_getPath(oNChild)));
         }
      }

      printf("BLOCK END\n\n\n\n");
      */
      /* Sample check on each node: node must be valid */
      /* If not, pass that failure back up immediately */
      if(!CheckerDT_Node_isValid(oNNode))
         return -1;

      if (!CheckerDT_hasValidChildren(oNNode)) {
         fprintf(stderr, "At least two children have identical path or are out of order\n");
         return -1;
      }

      /* Recur on every child of oNNode */
      for(ulIndex = 0; ulIndex < Node_getNumChildren(oNNode); ulIndex++)
      {
         Node_T oNChild = NULL;
         int iStatus = Node_getChild(oNNode, ulIndex, &oNChild);

         if(iStatus != SUCCESS) {
            fprintf(stderr, "getNumChildren claims more children than getChild returns\n");
            return -1;
         }

         /* Verify that the parent of the child is correct
         if (oNNode != Node_getParent(oNChild)) {
            fprintf(stderr, "parent of child elemenet is invalid\n");
         }*/

         /* if recurring down one subtree results in a failed check
            farther down, passes the failure back up immediately */
         iNodeCount = CheckerDT_treeCheck(oNChild, iNodeCount);

         if (iNodeCount == -1)
            return iNodeCount;
      }
   }
   return iNodeCount;
}

/* see checkerDT.h for specification */
boolean CheckerDT_isValid(boolean bIsInitialized, Node_T oNRoot,
                          size_t ulCount) {

   /* Sample check on a top-level data structure invariant:
      if the DT is not initialized, its count should be 0. */
   if(!bIsInitialized)
      if(ulCount != 0) {
         fprintf(stderr, "Not initialized, but count is not 0\n");
         return FALSE;
      }

   /* Now checks invariants recursively at each node from the root. */
   int iNodeCount = CheckerDT_treeCheck(oNRoot, 0);
   /*printf("iNodeCount: %i; ulCount: %u\n", iNodeCount, ulCount);*/

   if (iNodeCount == -1)
      return FALSE;
   
   if (iNodeCount != ulCount) {
      fprintf(stderr, "ulCount does not match the number of nodes in the tree\n");
      return FALSE;
   }
   
   return TRUE;
}
