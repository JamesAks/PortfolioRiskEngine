#ifndef ASSET_HPP
#define ASSET_HPP

#include <string>
#include <vector>
#include <memory>



class HistoricData;
class TimeSeries;
enum class TimeFrame;


class Asset {

    // Very high-level, base class for an asset. Essentially an assets symbol(ticker name) and its historical data. Could be an option, bond etc.

    private:

        std::string asset_symbol;
        std::shared_ptr<double> latest_price;
        std::shared_ptr<HistoricData> hist_data;

    public:
        

        Asset(std::string, std::shared_ptr<double>, std::shared_ptr<HistoricData>);

        // Returns the current/latest price of the asset.
        double latestPrice() const;

        // Returns historic data of asset.
        const HistoricData& historicData() const;

        // Returns specific periodic data for hsitoric data.
        const TimeSeries& periodicData(TimeFrame) const;

        // Returns the symbol/ID if the asset.
        std::string symbol() const;  
};

#endif // !ASSET_HPP