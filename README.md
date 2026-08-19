# PortfolioRiskEngine

A C++ app/tool for basic risk analysis.

This project aims to create a tool that implements financial theorys to calculate analytics such as volatility using historical data.

My aim is to explore quantitative finance theories and concepts while refreshing and refining my C++ skills. 


## Architecture

The project follows a structure of a quantitative library that contains the business logic of the tool and executables that use the library.

Interface -----> Portfolio Service -----> Risk Engine -----> Data Models -----> Pricing Engine

![Architecture](docs\images\ArchitectureDiagramV3.drawio.png)

![Planning](docs/images/Planning.md)


## How to use:

The application relies on using a data provider, Alpha Vantage, that requires an API key. In order to use the app an API key is needed. AlphaVantage provides a free API key on its site that can be obtain once signing up. 

----> https://www.alphavantage.co/support/#api-key <----- Alpha Vantage API Key

Step 1 ---> Get Alpha Vantage API key.

Step 2 ---> Create a system variable called "ALPHA_VANTAGE_API_KEY". (To be changed in later versions).

Step 3 ---> Download latest release zip file and extract.

Step 4 ---> Run executable.


## Example use

![Starting screen](docs\images\default_screen.png)

Using the application is quite simple. Click on the "Add Portfolio" button and enter a name to get started!
![Portfolio creation](docs\images\create_portfolio.png)

The application supports multiple portfolios at the same time.
![Multiple Portflios](docs\images\multiple_portfolios.png)

Add a position to your portfolio by filling in the fields and specifying an underlying asset. This only works with stocks currently but there are plans to add more asset types soon!
![Add Position](docs\images\add_asset.png)

Like the Portfolios the application supports multiple assets.
![Multiple Assets](docs\images\multiple_assets.png)

With your assets added all thats left is to choose your timeframe and analyse your portfolio.
![Timeframe](docs\images\set_timeframe.png)
![Analysis](docs\images\full_analysis.png)

In the background the market data is being updated at a constant rate (1 minute by default). WARNING - This uses alphavantage's api. If you are using the free version and have a lot of assets you will burn through your limit almost INSTANTLY.
![Logs](docs\images\auto_update.png)

Thats it for now but theres more to come such as more asset types, optimal portfolio generation, and simulations.


## Repository Structure

Apps/       # Applications that make use of the quantitative library

Assets/     # Contains the data models for financial instruments

Core/       # Contains objects that are used throughout every sub-library

DataLoader/     # Utility class for downloading csv data

docs/               # Diagrams, design notes, planning

MarketDataServices/     # Contains objects needed for data storage and data retrieval

out/        # Build diractory

PricingEngine/      # Contains the engines used for pricing financial instruments

tests/      # Contains financial validation, intergration and unit tests

CMakeLists.txt      # Build configuration

README.md           # Main documentation

.gitignore


## Future Features

    - Asset Types (Bonds, Options and more)
    - Monte Carlo portfolio simulation
    - Stress Testing
