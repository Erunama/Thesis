import plotly.express as px
import pandas as pd
import os
from glob import glob

PATH = "./Results/"
EXT = "*.csv"
all_csv_files = [file
                 for path, subdir, files in os.walk(PATH)
                 for file in glob(os.path.join(path, EXT))]

# TODO: Add some variable data,
# TODO: Graph swapping maybe?
# dfs = []
# for file in glob.glob("./results/*.csv"):
#     dfs.append(pd.read_csv(file))
dfs = []
for file in all_csv_files:
    dfs.append(pd.read_csv(file))
result = pd.concat(dfs)


# assume you have a "long-form" data frame
# see https://plotly.com/python/px-arguments/ for more options
# df = pd.read_csv('result2s.csv')
result["cca"] = result["cca"].astype(str)
result["density"] = result["density"].astype(str)

grouped = result.groupby(result.density)

res_low_dens = grouped.get_group("10")
res_high_dens = grouped.get_group("20")

print(result.head())
fig1 = px.scatter(result, x='size', y='total_time', color="cca",
                  hover_data=['density'], title="Plot of all 7 Basic Versions",
                  symbol="density",
                  trendline='lowess',
                  labels={"total_time": "Time for 10000 Iterations",
                          "size": "Graph Size",
                          "cca": "Connected Components Version",
                          "density": "Density"},
                  category_orders={"cca": ["1", "2", "3", "4", "5", "6", "7"],
                                   "density": ["10", "20"]})

fig2 = px.scatter(res_low_dens, x='size', y='total_time', color="cca",
                  hover_data=['density'],
                  title="Plot of all 7 current implementations, Density=10",
                  trendline='lowess',
                  labels={"total_time": "Time for 10000 Iterations",
                          "size": "Graph Size",
                          "cca": "Connected Components Version",
                          "density": "Density"},
                  category_orders={"cca": ["1", "2", "3", "4", "5", "6", "7"]

                                   })

fig3 = px.scatter(res_high_dens, x='size', y='total_time', color="cca",
                  hover_data=['density'],
                  title="Plot of all 7 current implementations, Density=20",
                  trendline='lowess',
                  labels={"total_time": "Time for 10000 Iterations",
                          "size": "Graph Size",
                          "cca": "Connected Components Version",
                          "density": "Density"},
                  category_orders={"cca": ["1", "2", "3",
                                           "4", "5", "6", "7"]})


# fig4 = px.scatter(result, x='size', y='total_time', color="density",
#                   animation_frame="cca",
#                   hover_data=['density'],
#                   title="Interactive plot of CCA performence over iterations",
#                   symbol="density",
#                   trendline='lowess',
#                   labels={"total_time": "Time for 10000 Iterations",
#                           "size": "Graph Size",
#                           "cca": "Connected Components Version",
#                           "density": "Density"},
#                   category_orders={"cca": ["1", "2", "3", "4", "5", "6", "7"],
#                                    "density": ["10", "20"]})

# fig5 = px.scatter(result, x='size', y='total_time', color="cca",
#                   animation_frame="density",
#                   hover_data=['density'],
#                   title="Interactive plot of CCA performence over density",
#                   symbol="density",
#                   trendline='lowess',
#                   labels={"total_time": "Time for 10000 Iterations",
#                           "size": "Graph Size",
#                           "cca": "Connected Components Version",
#                           "density": "Density"},
#                   category_orders={"cca": ["1", "2", "3", "4", "5", "6", "7"],
#                                    "density": ["10", "20"]})


fig1.show()
fig2.show()
fig3.show()

# Interactive versions
# fig4.show()
# fig5.show()
