#include "Asset.hpp"
using namespace std;

class Portfolio{

    private:

        std::vector<Asset> assets;


    public:

        Portfolio();
        Portfolio(vector<Asset>);


        void add(Asset);
        void remove(string);
        size_t size();

        void update();

};
