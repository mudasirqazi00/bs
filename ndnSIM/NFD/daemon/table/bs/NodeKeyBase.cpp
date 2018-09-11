
#include <ostream>
#include <string>

namespace nfd {
	namespace cs {
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
				virtual void print( std::ostream& out);
				friend std::ostream& operator<<( std::ostream& out, NodeKeyBase& b )
				{
					b.print( out );
					return out;
				}		
		};
	}
}