#include <list>
#include <Directory.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <Path.h>
#include <stdio.h>

using std::list;

int main(void)
{
	BPath path;
	find_directory(B_USER_DIRECTORY, &path);
	BDirectory dir(path.Path());
	
	list<entry_ref> refList;
	
	entry_ref ref;
	while(dir.GetNextRef(&ref) == B_OK)
		refList.push_back(ref);

	printf("Contents of the home folder: %s\n", path.Path());
	for(list<entry_ref>::iterator i = refList.begin(); i!=refList.end(); i++)
		printf("\t%s\n", i->name);
}