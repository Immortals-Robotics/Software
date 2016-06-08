/*
 * TITLE:      ConfigReader.cc
 *
 * PURPOSE:    This class can be used as a general configuration file reader
 *             ala .ini reading files.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "configreader.h"

// The Global Reader
ConfigReader configreader = ConfigReader();

// maximum length of line
#define MAX_LINE 1024
#define ISCOMMA(c)         ((c) == ',')
#define ISSPACE(c)         (((c) == ' ') || ((c) == '\t'))
#define EOSTR(sptr)     (*(sptr) == '\0')

#define MAX_VECCOUNT     30

DataMap::DataMap ()
{
	fname      = 0;
	dataname   = 0;
	data.ddata = 0;
}

DataMap::~DataMap ()
{
	delete [] fname;
	delete [] dataname;
	delete [] data.ddata;
}

ConfigFile::Element::Element ()
{
	string    = 0;
	data      = 0;
	str_size  = 0;
	data_size = 0;
}

ConfigFile::Element::Element (const Element& e)
{
	str_size  = e.str_size;
	data_size = e.data_size;
	
	string = new char[str_size + 1];
	data   = new char[data_size + 1];

	strcpy (string, e.string);
	strcpy (data,   e.data);
}

ConfigFile::Element::~Element ()
{
	delete [] string;
	delete [] data;
}

ConfigFile::ConfigFile(void)
{
  elements.clear();
}

ConfigFile::~ConfigFile(void)
{
  elements.clear();
}

void ConfigFile::Chomp(char *str)
{
  while (!EOSTR(str)) {
    if (*str == '\n') {
      *str = '\0';
      return;
    } 
    str++;
  }
}


// utilities for operating on strings
char *ConfigFile::GetNextEntry(char *str)
{
  // scroll through until we reach end or find whitespace
  while (!EOSTR(str) && !(ISSPACE(*str) || ISCOMMA(*str)))
    str++;
  if (EOSTR(str))
    return (NULL);

  // now find good entry
  while (!EOSTR(str) && (ISSPACE(*str) || ISCOMMA(*str)))
    str++;
  if (EOSTR(str))
    return (NULL);

  return (str);
}


bool ConfigFile::AddElement(char *str, char *data)
{
  Element e;

  e.str_size = strlen(str);

  // data has double terminator
  e.data_size = strlen(data) + 1;

  e.string = new char[e.str_size + 1];
  e.data   = new char[e.data_size + 1];
  strcpy(e.string, str);
  strcpy(e.data, data);
  e.data[e.data_size - 1] = '\0';

  /* add it to the list in sorted order */
  elements.push_back(e);
  return (true);
}


void ConfigFile::DeleteList(void)
{
/*
  for(uint i=0; i<elements.size(); i++) {
    delete [] elements[i].string;
    delete [] elements[i].data;
  }
*/    
  elements.clear();
}

// remove white space from start of string
char *ConfigFile::StripFront(char *start)
{
  while (!EOSTR(start)) {
    if (!ISSPACE(*start))
      return (start);
    start++;
  }
  return (NULL);
}

// remove white space from start of string
bool ConfigFile::StripTail(char *start)
{
  char *lastpt = NULL;

  // find the end of the string
  while (!EOSTR(start)) {
    if (!ISSPACE(*start))
      lastpt = start;
    start++;
  }

  //last pt should point to last non-space element -> terminate it
  if (lastpt == NULL)
    return (false);
  *(++lastpt) = 0;
  return (true);
}


// add null character at first white space in string
bool ConfigFile::TerminateString(char *start)
{
  while (*start != 0) {
    if (ISSPACE(*start)) {
      *start = 0;
      return (true);
    }
    start++;
  }
  return (false);
}


bool ConfigFile::ReadFile(char *fname)
{
  FILE *f;
  char line[MAX_LINE];
  char *eqptr;
  char *string, *data;

  // clear old list
  DeleteList();

  // open the file
  if ((f = fopen(fname, "rt")) == NULL)
    return (false);

  while (fgets(line, MAX_LINE, f) != NULL) {
    if ((line[0] != '#') && ((eqptr = strchr(line, '=')) != NULL)) {

      // remove linefeed
      Chomp(line);

      // remove leading spaces
      *eqptr = 0;
      if ((string = StripFront(line)) == NULL)
	continue;
      TerminateString(string);

      // get data & remove leading and trailing spaces
      data = eqptr + 1;
      if ((data = StripFront(data)) == NULL)
      	continue;
      StripTail(data);

      // add the string to the list here
      //      fprintf(stderr, "string (%s) => data (%s)\n", string, data);

      AddElement(string, data);
    }
  }

  // all done
  fclose(f);
  return (true);
}


// tries to find the element string in the tree
// if successful returns a pointer to the corresponding
// data, if not returns null
char *ConfigFile::FindString(char *str)
{
  for (unsigned int i = 0; i < elements.size(); i++) {
    if (strcmp(elements[i].string, str) == 0)
      return (elements[i].data);
  }
  return (NULL);
}

void ConfigFile::Print(FILE *f)
{
  for (unsigned int i = 0; i < elements.size(); i++) {
    fprintf(f, "%s = %s_\n", elements[i].string, elements[i].data);
  }
}


int ConfigFile::ReadVector(DataMap *dmap, char *str, int _type)
{
  double* dptr = new double[MAX_VECCOUNT];
  int*    iptr = new int[MAX_VECCOUNT];

  int count = 0;
  char *cptr = str, *eptr = str;

  // find all the vectors in the list
  dmap->veccount = 0;
  dmap->data.ddata = NULL;
  dmap->data.idata = NULL;
  dmap->type = _type;
  do {
    cptr = eptr;

    switch (_type) {
    case CR_DOUBLE:
      dptr[count++] = strtod(cptr, &eptr);
      break;
    case CR_INT:
      iptr[count++] = strtol(cptr, &eptr, 0);
      break;
    case CR_STRING:
    case CR_CHAR:
      count = strlen(str);

      // break out of the while loop
      eptr = str + count;
      break;
    default:
      return 0;
    }
#ifdef DEBUG
    fprintf(stderr, "extract %s -> %s\n", cptr, eptr);
#endif

    if (eptr != cptr) {
      while ((*eptr != 0) && !isdigit(*eptr))
	eptr++;
    }
  } while ((*eptr != 0) && (eptr != cptr));

  // did we find anything
  if (count == 0) {
    delete [] dptr;
    delete [] iptr;
    return count;
  }

  dmap->veccount = count;

  switch (_type) {
  case CR_DOUBLE: {
    dmap->data.ddata = new double[count];
    memcpy(dmap->data.ddata, dptr, sizeof(double) * count);
    dmap->vecsize = sizeof(double);
    } break;
  case CR_INT: {
    dmap->data.idata = new int[count];
    memcpy(dmap->data.idata, iptr, sizeof(int) * count);
    dmap->vecsize = sizeof(int);
    } break;
  case CR_STRING:
  case CR_CHAR:
    {
    dmap->data.cdata = new char[count + 1];
    strcpy(dmap->data.cdata, str);
    dmap->vecsize = sizeof(char);
    } break;
  }

  delete [] dptr;
  delete [] iptr;
  return (count);
}


int ConfigFile::ReadString(DataMap *dmap, char *str)
{
  return 0;
}


/******************************** ConfigReader ************************/

ConfigReader::ConfigReader(void)
{
  path = DATA_PATH;
}

ConfigReader::~ConfigReader(void)
{
  unsigned int i;
	
	for (i = 0; i < filelist.size (); i++)
	{
		delete [] filelist[i];
	}

	for (i = 0; i < datamap.size (); i++)
	{
		delete datamap[i];
	}
}
    
bool ConfigReader::ReadFile(const char *const _fname)
{
  static char filepath[1024];

  snprintf(filepath, 1024, "%s/%s.cfg", path, _fname);

  int reader_i = FindFile(_fname);

  if (reader_i >= 0) {
    if (!filereaders[reader_i].ReadFile(filepath)) {
      fprintf(stderr, "ERROR: Cannot reread config file %s.\n", filepath);
      return false;
    } 

    return true;
  }

  ConfigFile cf;
  if (!cf.ReadFile(filepath)) {
    fprintf(stderr, "ERROR: Cannot read config file %s.\n", filepath);
    return false;
  }

  char *names = new char[strlen(_fname) + 1];
  strcpy(names, _fname);

  filelist.push_back(names);
  filereaders.push_back(cf);

  return true;
}

bool ConfigReader::UpdateDataMap(PDataMap dmap)
{
  int reader_i = FindFile(dmap->fname);
  if (reader_i < 0) return false;;

  char *datastr = filereaders[reader_i].FindString(dmap->dataname);
  if (!datastr) return false;

  int rval = filereaders[reader_i].ReadVector(dmap, datastr, dmap->type);
  if (!rval) return false;

  return true;
}

// reread the file list and update the mapping tables
void ConfigReader::UpdateFiles(void)
{
  for(unsigned int i=0; i<filelist.size(); i++)
    ReadFile(filelist[i]);

  for(unsigned int i=0; i<datamap.size(); i++) {
    if (!UpdateDataMap(datamap[i])) {
      fprintf(stderr, "WARNING: Config data %s:%s now missing.\n", 
	      datamap[i]->fname, datamap[i]->dataname);
    }
  }
}


DataMap *ConfigReader::FindDataMapping(const char * const _fname, const char *const _dataname, int _type)
{
  // Make sure the file has been read.
  if (FindFile(_fname) < 0) ReadFile(_fname);

  // See if it has already been mapped
  for (unsigned int i = 0; i < datamap.size(); i++) {
    if ((strcmp(_fname, datamap[i]->fname) == 0) &&
	(strcmp(_dataname, datamap[i]->dataname) == 0)) {
      return (datamap[i]);
    }
  }

  // Create a data map for the entry.
  DataMap *dmap = new DataMap;
  dmap->cached = true;
  dmap->type = _type;

  dmap->data.ddata = NULL;
  dmap->data.idata = NULL;
  dmap->vecsize = 0;

  dmap->fname = new char[strlen(_fname) + 1];
  strcpy(dmap->fname, _fname);
  dmap->dataname = new char[strlen(_dataname) + 1];
  strcpy(dmap->dataname, _dataname);

  // Try to find it and parse it.
  if (!UpdateDataMap(dmap)) {
    fprintf(stderr, "WARNING: Config data %s:%s does not exist.\n",
	    _fname, _dataname);  
    delete [] dmap->fname;
    delete [] dmap->dataname;
    delete dmap;
    return NULL;
  }

  // If we found it add it to the data map;
  datamap.push_back(dmap);

  // all done
  return (dmap);
}


void ConfigReader::Print(FILE *f)
{
  fprintf(f, "Filelist\n");
  for (unsigned int i = 0; i < filelist.size(); i++)
    fprintf(f, "\t%s\n", filelist[i]);
  fprintf(f, "\n");

  fprintf(f, "Datamapping\n");
  for (unsigned int i = 0; i < datamap.size(); i++) {
    fprintf(f, "%s:%s cached %i\n", datamap[i]->fname, 
	    datamap[i]->dataname, datamap[i]->cached);
    fprintf(f, "\tsz %i\ttp %i D: ", datamap[i]->veccount, datamap[i]->type);
    switch (datamap[i]->type) {
    case CR_DOUBLE: {
      double *dptr = (double *) datamap[i]->data.ddata;
      for (unsigned int j = 0; j < datamap[i]->veccount; j++)
	fprintf(f, "%f, ", dptr[j]);
    } break;
    case CR_INT: {
      int *iptr = (int *) datamap[i]->data.idata;
      for (unsigned int j = 0; j < datamap[i]->veccount; j++)
	fprintf(f, "%i, ", iptr[j]);
    } break;
    case CR_CHAR:
    case CR_STRING:
      fprintf(f, "%s", datamap[i]->data.cdata);
      break;
    }
    fprintf(f, "\n");
  }
}



#ifdef TEST_MAIN
CR_DECLARE(YELLOW_ENABLE);

int main(int argc, char *argv[]) 
{
  PDataMap d1, d2, d3, d4, d5, d6, d7;

  d1 = configreader.FindDataMapping("test", "data1", CR_DOUBLE, 3);
  d2 = configreader.FindDataMapping("test", "data2", CR_INT);
  d7 = configreader.FindDataMapping("test", "data2", CR_INT);
  d3 = configreader.FindDataMapping("test", "data3", CR_STRING);
  d4 = configreader.FindDataMapping("test2", "data1", CR_INT);
  d5 = configreader.FindDataMapping("test2", "data2", CR_DOUBLE);
  d6 = configreader.FindDataMapping("test2", "other", CR_STRING);

  CR_SETUP(gui, YELLOW_ENABLE, CR_DOUBLE);

  configreader.Print(stderr);

  if (!(d1 && d2 && d3 && d4 && d5 && d6)) {
    fprintf(stderr, "Cannot find all the data!\n");

    delete d1;
    delete d2;
    delete d3;
    delete d4;
    delete d5;
    delete d6;
    delete d7;
    
    exit(1);
  }

  // lets do some data access
  fprintf(stderr, "Array 1 %i: ", VARSIZE(d1));
  for (uint i = 0; i < VARSIZE(d1); i++)
    fprintf(stderr, "%f, ", VDVAR(d1)[i]);
  fprintf(stderr, "\nArray 2: ");
  for (uint i = 0; i < VARSIZE(d2); i++)
    fprintf(stderr, "%d, ", VIVAR(d2)[i]);
  fprintf(stderr, "\ndata 3 %s\n", SVAR(d3));
  fprintf(stderr, "data 6 %s\n", SVAR(d6));

  delete d1;
  delete d2;
  delete d3;
  delete d4;
  delete d5;
  delete d6;
  delete d7;

  return (0);
}
 
#endif


