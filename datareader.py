import plotly.express as px
import pandas as pd
import glob

# TODO: Add some variable data,
# TODO: Graph swapping maybe?
dfs = []
for file in glob.glob("./results/*.csv"):
    dfs.append(pd.read_csv(file))

result = pd.concat(dfs)


# assume you have a "long-form" data frame
# see https://plotly.com/python/px-arguments/ for more options
# df = pd.read_csv('result2s.csv')
result["cca"] = result["cca"].astype(str)
result["density"] = result["density"].astype(str)
print(result.head())
fig = px.scatter(result, x='size', y='total_time', color="cca",
                 hover_data=['density'], title="Plot of all results",
                 symbol="density",
                 labels={"total_time": "Time for 10000 Iterations",
                         "size": "Graph Size",
                         "cca": "Connected Components Version",
                         "density": "Density"})
fig2 = px.scatter(result, x='total_time', y='size', color="cca",
                  hover_data=['density'], title="Plot of all results",
                  symbol="density",
                  labels={"total_time": "Time for 10000 Iterations",
                          "size": "Graph Size",
                          "cca": "Connected Components Version",
                          "density": "Density"})
# fig = px.scatter_matrix(result,
#                         dimensions=["total_time", "size", "density"],
#                         color="cca")
fig.show()
fig2.show()
