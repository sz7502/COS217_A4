/*--------------------------------------------------------------------*/
/* checkerDT.c                                                        */
/* Author: Stan Zhelokhovtsev                                         */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "checkerDT.h"
#include "dynarray.h"
#include "path.h"


struct TreeData {
   size_t ulCount;
   boolean valid;
} typedef TreeData;


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

/* Ensures that all children of oNNode do not share identical paths and
   that they are in order. If they do not meet the criteria, return
   FALSE. Otherwise, return TRUE. */
static boolean CheckerDT_Node_hasValidChildren(Node_T oNNode) {
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
         int compareResult;
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

         compareResult = strcmp(oNChildStringPath, oNChild2StringPath);

         if (compareResult == 0) {
            fprintf(stderr, "Atleast two children of a node have " 
                            "identical paths\n");
            return FALSE;
         }
         if (compareResult >= 0) {
            fprintf(stderr, "Atleast two children of a node are out of "
                            "lexicographic order\n");
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
static void CheckerDT_treeCheck(Node_T oNNode, TreeData *treeData) {
   size_t ulIndex;

   if(oNNode!= NULL) {
      treeData->ulCount++;
      
      /* Sample check on each node: node must be valid */
      /* If not, pass that failure back up immediately */
      if(!CheckerDT_Node_isValid(oNNode))
         treeData->valid = FALSE;
         return;

      if (!CheckerDT_Node_hasValidChildren(oNNode)) {
         treeData->valid = FALSE;
         return;
      }

      /* Recur on every child of oNNode */
      for(ulIndex = 0; ulIndex < Node_getNumChildren(oNNode); ulIndex++)
      {
         Node_T oNChild = NULL;
         int iStatus = Node_getChild(oNNode, ulIndex, &oNChild);

         if(iStatus != SUCCESS) {
            fprintf(stderr, "getNumChildren claims more children than getChild returns\n");
            treeData->valid = FALSE;
            return;
         }

         /* if recurring down one subtree results in a failed check
            farther down, passes the failure back up immediately */
         CheckerDT_treeCheck(oNChild, treeData);
         if (!treeData->valid)
            return;
      }
   }
}

/* see checkerDT.h for specification */
boolean CheckerDT_isValid(boolean bIsInitialized, Node_T oNRoot,
                          size_t ulCount) {
   TreeData treeData;
   treeData.ulCount = 0;
   treeData.valid = TRUE;

   /* Sample check on a top-level data structure invariant:
      if the DT is not initialized, its count should be 0. */
   if(!bIsInitialized)
      if(ulCount != 0) {
         fprintf(stderr, "Not initialized, but count is not 0\n");
         return FALSE;
      }

   /* Now checks invariants recursively at each node from the root. */
   CheckerDT_treeCheck(oNRoot, &treeData);

   if (!treeData.valid)
      return FALSE;
   
   if (treeData.ulCount != ulCount) {
      fprintf(stderr, "ulCount does not match the number of nodes in the tree\n");
      return FALSE;
   }
   
   return TRUE;
}
