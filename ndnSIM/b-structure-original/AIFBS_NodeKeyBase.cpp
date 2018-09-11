/**
*	@description Every AIBFS_BTree must should have key type of follow
*/
#include <ostream>
#include <string>
namespace AIFBS {
	class AIFBS_NodeKeyBase
	{
		public:
			std::string key;			
    		AIFBS_NodeKeyBase() {
    			key = "";
    		}
    		virtual ~AIFBS_NodeKeyBase();
			virtual bool operator< (const AIFBS_NodeKeyBase&)=0;
			virtual bool operator> (const AIFBS_NodeKeyBase&)=0;
    		virtual void Print( std::ostream& out);
			friend std::ostream& operator<<( std::ostream& out, AIFBS_NodeKeyBase& b )
		    {
		        b.Print( out );
		        return out;
		    }		
	};
}