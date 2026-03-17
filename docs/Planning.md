# Planning

This is a document containing some of my planning for this project

# User Stories

    - The user should be able to create a portfolio

    - The user should be able to add assets into their portfolios

    - The user should be able to name an asset, specify its type, and record its cashflows.

    - The user should be able to input files and get files as outputs 

    - User should be able to calculate and retrieve data such as historical return calculations,correlation matrices and portfolio variance.

    - The user should be able to remove/add assets and be able to do calculations on the newly updated portfolio.

    - The user should be able to use various analytical formulas to analyse a portfolios risk/returns

    - The user should be able to access individual assets and update their data.


# Minimum Viable Product

    - A user should be able to interact with the command line 

    - A user should be able to upload a csv file to create a portfolio

    - A user should be able to upload a csv file to create an asset

    - A user should be able manipulate

    - A program should be able to calculate basic analytics like expected return


# Overall Development Process

    1. Create Data Models
        - Portfolio Class
        - Asset Class
        - Risk Engine

    2. Build Statistics Module
    
    3. Build Risk Engine
        - expectedReturn()
        - volatility()
        - correlationMatrix()

    4. Build Application Layer
        - PortfolioManager

    5. Build Interface
        - Command Line input
        - Parse user command