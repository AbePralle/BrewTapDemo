#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <libgen.h>
#include <unistd.h>
using namespace std;

// get_executable_filepath() based on
// https://stackoverflow.com/a/60250581/135791

#if defined(__APPLE__)
  #include <mach-o/dyld.h>

  void get_raw_executable_filepath( char* buffer )
  {
    uint32_t buffer_size = (uint32_t) PATH_MAX;
    if (0 != _NSGetExecutablePath(buffer,&buffer_size))
    {
      buffer[0] = 0;
    }
  }

#elif defined(__linux__)
  void get_raw_executable_filepath( char* buffer )
  {
    #if defined(__sun)
      strcpy( buffer, "/proc/self/path/a.out" );
    #else
      strcpy( buffer, "/proc/self/exe" );
    #endif
  }

#endif

void get_executable_filepath( char* buffer )
{
  // 'buffer' must be >= PATH_MAX in size.
  char raw_filepath[PATH_MAX];
  get_raw_executable_filepath( raw_filepath );
  realpath( raw_filepath, buffer );
}

int main( int argc, char* argv[] )
{
  char data_filepath[PATH_MAX];
  get_executable_filepath( data_filepath );

  int found_slashes = 0;
  for (int i=strlen(data_filepath); --i>=0; )
  {
    if (data_filepath[i] == '/')
    {
      if (++found_slashes == 2)
      {
        data_filepath[i] = 0;
        break;
      }
    }
  }
  strcat( data_filepath, "/Data/Greeting.txt" );

  FILE* fp = fopen( data_filepath, "r" );
  if (fp)
  {
    for (int ch=getc(fp); ch!=-1; ch=getc(fp))
    {
      printf( "%c", ch );
    }
    fclose( fp );
    return 0;
  }
  else
  {
    printf( "ERROR - couldn't open %s\n", data_filepath );
    return 1;
  }
}

