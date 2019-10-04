#pragma once

#define assert_eq(exp, value)\
if ( (exp != value) ) error(1, 0000, "%s:%d: Expected "#exp " == "#value " but found "#exp " == %d \n",__FILE__,__LINE__, exp); \
else printf("%s:%d Success "#exp " == "#value "\n", __FILE__,__LINE__);

