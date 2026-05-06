#include <string>
#include <vector>

class Asset {

    // Very high-level, base class for an asset. Essentially an assets symbol(ticker name) and its historical data. Could be an option, bond etc.

    private:

        std::string asset_symbol;
        double current_price;

    public:
        

        Asset(std::string, double);

        // Returns the current/latest price of the asset.
        double currentPrice();

        // Updates the current price of the asset.
        void updatePrice(double);

        // Returns the symbol/ID if the asset.
        std::string symbol();  
};
