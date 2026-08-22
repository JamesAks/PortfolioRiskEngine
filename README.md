# PortfolioRiskEngine

A C++ app/tool for basic risk analysis.

A tool for analysing user made portfolios, provide risk metrics such as volaitility and sharpe ratio. 

My aim is to explore quantitative finance theories and concepts while refreshing and refining my C++ skills. 

# About this project

So far, this project has:

Portfolio Management - User's can create, remove and modify portfolios that hold positions (both long and short). Portfolios rememver their held positions/assets and are given user generated names.

Portfolio Analysis - Users can analyse portfolios at different timeframes (daily, weekly and monthly) and produce risk metrics such as volatility, sharpe ratio, historical VaR and more.

Data Visualisation - Important graphs such as the efficient frontier are visualised on graphs, allowing the user to better understand their portfolio.


## Architecture

The project follows a structure of a quantitative library that contains the business logic of the tool and executables that use the library.

<img width="1832" height="900" alt="ArchitectureDiagramV3 drawio" src="https://github.com/user-attachments/assets/fb1d7c37-e650-4163-976d-0f26348e7d9f" />


## Getting Started:

The application relies on using a data provider, Alpha Vantage, that requires an API key. In order to use the app an API key is needed. AlphaVantage provides a free API key on its site that can be obtain once signing up. 

----> https://www.alphavantage.co/support/#api-key <----- Alpha Vantage API Key

Step 1 ---> Get Alpha Vantage API key.

Step 2 ---> Create a system variable called "ALPHA_VANTAGE_API_KEY". (To be changed in later versions).

Step 3 ---> Download latest release zip file and extract.

Step 4 ---> Run executable.


## How to use

This is the landing screen.

<img width="1714" height="835" alt="default_screen" src="https://github.com/user-attachments/assets/a55ac5e7-8f86-4df7-99e6-c8e554eb8163" />


Getting started requires a portfolio to be created. Click on the "Add Portfolio" button and enter a name to get started!

<img width="544" height="426" alt="create_portfolio" src="https://github.com/user-attachments/assets/f95e62fd-616b-409a-8fe5-2ec98bdec338" />


The application supports multiple portfolios at the same time.

<img width="314" height="161" alt="multiple_portfolios" src="https://github.com/user-attachments/assets/a20c9590-07fd-48e6-a435-92d288aeb374" />


Add a position to a portfolio by selecting the desired portfolio and clicking the "Add Position" button. Fill in the fields, press the create button and a position is created. This only works with stocks currently but there are plans to add more asset types soon!

<img width="777" height="498" alt="add_asset" src="https://github.com/user-attachments/assets/cbee003a-496d-4d5b-9a10-e3e53be39ddb" />

Clicking on a position also brings up an analysis window, showing a date/returns graph, allowing an in-depth analysis of the underlying assets performance.

<img width="1004" height="724" alt="asset_analysis" src="https://github.com/user-attachments/assets/17b1b59d-212c-4865-856d-d819f46d9b36" />


Like the Portfolios the application supports multiple assets.

<img width="1057" height="283" alt="multiple_assets" src="https://github.com/user-attachments/assets/fb6c2a3b-55b0-41b6-9f88-3141df471628" />


With your assets added all thats left is to choose your timeframe and analyse your portfolio.

<img width="396" height="185" alt="set_timeframe" src="https://github.com/user-attachments/assets/2dca5e73-2339-4e43-bb09-1364899a6fbd" />

<img width="1673" height="753" alt="full_analysis" src="https://github.com/user-attachments/assets/76ca1132-e978-4c24-9a92-2931362d55c9" />


In the background the market data is being updated at a constant rate (1 minute by default). WARNING - This uses Alpha Vantages api. If you are using the free version and have a lot of assets you WILL burn through your limit almost INSTANTLY.

<img width="899" height="96" alt="auto_update" src="https://github.com/user-attachments/assets/6fb50d08-ca57-4cdd-bcaa-691414e7df70" />



Thats it for now but theres more to come, such as, more asset types, optimal portfolio generation, and simulations.


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
