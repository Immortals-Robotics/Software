/*
 * TITLE:      ConfigReader.h
 *
 * PURPOSE:    This class can be used as a general configuration file reader
 *             ala .ini reading files. It will read file formats as follows:
 *             # - comment character. Anything afterwards is ignored
 *               - blank lines are ignored
 *             <identifier> <ignored stuff> = <data>  <data2> ... <datan>
 *
 *             strings after teh identifier (first space after text)
 *             is ignored anything after the equal sign, ignoring
 *             leading spaces is stored as a string. Any characters
 *             other than spaces (and tabs) are legal for the
 *             identifier as limited by isspace() function
 *
 *             Call the ReadFile function to parse the file. Call the
 *             FindString funciton with your identifier to retrieve
 *             hte string data on the right hand side of equal sign
 *             ignoring leading and trailing spaces.
 *
 *             Print function will output the data stored in the list
 *             for debugging purposes
 *
 *             All data structures allocated are cleaned up on destruction
 *
 * WRITTEN BY: Brett Browning
 */
/* LICENSE:
  =========================================================================
    CMDragons'02 RoboCup F180 Source Code Release
  -------------------------------------------------------------------------
    Copyright (C) 2002 Manuela Veloso, Brett Browning, Mike Bowling,
                       James Bruce; {mmv, brettb, mhb, jbruce}@cs.cmu.edu
    School of Computer Science, Carnegie Mellon University
  -------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ------------------------------------------------------------------------- */

#ifndef __CONFIGREADER_H__
#define __CONFIGREADER_H__

#include <vector>

#include <stdio.h>

/* data types */
#define CR_DOUBLE      0
#define CR_INT         1
#define CR_STRING      2
#define CR_CHAR        3

// macros to access data variables
// xxVAR
// D - double data type
// I - int data type
// C - char type
// S - string type
// V - vector data type (no V means single data
#define DVAR(dmapptr)    (*((dmapptr)->data.ddata))
#define VDVAR(dmapptr)   ((dmapptr)->data.ddata)

#define IVAR(dmapptr)    (*((dmapptr)->data.idata))
#define VIVAR(dmapptr)   ((dmapptr)->data.idata)

#define CVAR(dmapptr)    (*((dmapptr)->data.cdata))
#define SVAR(dmapptr)    ((dmapptr)->data.cdata)

// macros for determining the size of an array
// -> a good sanity check
#define VARSIZE(dmapptr) ((dmapptr)->veccount)

// data map structure that stores everything in it
struct DataMap {
  bool cached;
  char *fname;
  int type;
  char *dataname;
  uint veccount;
  uint vecsize;
  union Data {
    double *ddata;
    int *idata;
    char *cdata;
  } data;

  DataMap  ();
  ~DataMap ();
};

typedef DataMap *PDataMap;


class ConfigFile {
private:
  struct Element {
    char *string;
    char *data;
    int str_size, data_size;

    Element  ();
    Element  (const Element& e);
    ~Element ();
  };

  vector<Element> elements;

  bool AddElement(char *str, char *data);
  void DeleteList(void);
  void Chomp(char *str);
  void ChopUp(char *str);
  
public:
  ConfigFile(void);
  ~ConfigFile(void);

  bool ReadFile(char *fname);

  // return the next entry (non-delimiter) in the string
  char *GetNextEntry(char *str);

  // remove white space from start of string
  char *StripFront(char *start);
  bool StripTail(char *start);

  // add null character at first white space in string
  bool TerminateString(char *start);

  // tries to find the element string in the tree
  // if successful returns a pointer to the corresponding
  // data, if not returns null
  char *FindString(char *str);
  void Print(FILE *f);
  void Print(void) {
    Print(stdout);
  }

  int ReadVector(DataMap *dmap, char *str, int _type);
  int ReadString(DataMap *dmap, char *str);
};

class ConfigReader {
private:
  vector<char *> filelist;
  vector<ConfigFile> filereaders;

  int FindFile(char *fname) {
    for(uint i=0; i<filelist.size(); i++)
      if (strcmp(filelist[i], fname) == 0) return i;
    return -1;
  }

  ConfigFile *FindReader(char *fname) {
    int i = FindFile(fname);
    if (i >= 0) return &filereaders[i];
    return NULL;
  }

  bool ReadFile(char *_fname);

  char *path;

public:
  vector<PDataMap> datamap;

  ConfigReader(void);
  ~ConfigReader(void);

  // Reread the file list and update the mapping tables
  void UpdateFiles(void);

  // Add a new data mapping
  DataMap *FindDataMapping(char *_fname, char *_dataname, int _type);

  // Update a data mapping (also used when first added)
  bool ConfigReader::UpdateDataMap(PDataMap dmap);

  // Add a new data mapping and also sanity check the number of vectors
  DataMap *FindDataMapping(char *_fname, char *_dataname, int _type, 
			   uint _count) {
    DataMap *rval = FindDataMapping(_fname, _dataname, _type);
    if (rval == NULL)
      return rval;
    if (rval->veccount < _count)
      return NULL;
    return rval;
  }

  void Print(FILE *f);
  void Print(void) { 
    Print(stdout); }

};

// The Global Reader
extern ConfigReader configreader;

// Quick Macros
#define CR_DECLARE(varname) static PDataMap varname;

#define CR_SETUP(filename,varname,type) \
  varname = \
    configreader.FindDataMapping(#filename, #varname, type); \
  if (!varname) abort();

#define CR_SETUP_CR(cr,filename,varname,type) \
  varname = \
    (cr).FindDataMapping("##filename", "##varname",type); \
  if (!varname) abort();


#endif /* __CONFIGREADER_H__ */

