/**
*	@description Every AIBFS_BTree must should have key type of follow
*/
#include <ostream>
#include <string>
namespace AIFBS {
	class NodeKeyBase
	{
		public:
			std::string key;			
    		NodeKeyBase() {
    			key = "";
    		}
    		virtual ~NodeKeyBase();
			virtual bool operator< (const NodeKeyBase&)=0;
			virtual bool operator> (const NodeKeyBase&)=0;
    		virtual void Print( std::ostream& out);
			friend std::ostream& operator<<( std::ostream& out, NodeKeyBase& b )
		    {
		        b.Print( out );
		        return out;
		    }		
	};
}