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
mxvs = []
for file in all_csv_files:
    p = pd.read_csv(file)
    if 'mxv_time' not in p.columns:
        dfs.append(p)
    if 'mxv_time' in p.columns and 'iteration' not in p.columns:
        mxvs.append(p)

result_t = pd.concat(dfs)
temp_list = [x for _, x in result_t.groupby(result_t.cca)]
result = pd.concat([temp_list[0], temp_list[1],
                    temp_list[4], temp_list[6], temp_list[7]])
result_t2 = pd.concat(mxvs)
temp_list2 = [x for _, x in result_t2.groupby(result_t2.cca)]
mxv_res = pd.concat([temp_list2[0], temp_list2[4],
                     temp_list2[2], temp_list2[3], temp_list2[1]])


# assume you have a "long-form" data frame
# see https://plotly.com/python/px-arguments/ for more options
# df = pd.read_csv('result2s.csv')

result["cca"] = result["cca"].astype(str)
result["density"] = result["density"].astype(str)

mxv_res["cca"] = mxv_res["cca"].astype(str)
mxv_res["density"] = mxv_res["density"].astype(str)
mxv_res["mxv_time"] = mxv_res["mxv_time"]/1000.0

# result2 = result[int(result['cca']) <= 3]
print(result.head())
grouped = result.groupby(result.density)

res_low_dens = grouped.get_group("10.0")
res_high_dens = grouped.get_group("20.0")
gp = mxv_res.groupby(mxv_res.density)

mxv_res_low_dens = gp.get_group("10")
mxv_res_high_dens = gp.get_group("20")

print(result.head())
fig1 = px.scatter(result, x='size', y='total_time', color="cca",
                  hover_data=['density'], title="Plot of all 7 Basic Versions",
                  symbol="density",
                  trendline='lowess',
                  log_y=True,
                  color_discrete_map={
                      "1": '#636EFA',
                      "2": '#EF553B',
                      "3": '#00CC96',
                      "4": '#AB63FA',
                      "5": '#FFA15A',
                      "6": '#19D3F3',
                      "7": '#FF6692',
                      "8": '#B6E880'
                  },
                  labels={"total_time": "Time for 10000 Iterations",
                          "size": "Graph Size",
                          "cca": "Connected Components Version",
                          "density": "Density"},
                  category_orders={"cca": ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"],
                                   "density": ["10", "20"]})

fig2 = px.scatter(res_low_dens, x='size', y='total_time', color="cca",
                  hover_data=['density'],
                  title="Plot total run time for reference implementation, Density=10",
                  trendline='lowess',
                  log_y=True,
                  color_discrete_map={
                      "1": '#636EFA',
                      "2": '#EF553B',
                      "3": '#00CC96',
                      "4": '#AB63FA',
                      "5": '#FFA15A',
                      "6": '#19D3F3',
                      "7": '#FF6692',
                      "8": '#B6E880'
                  },
                  labels={"total_time": "Time for 1000000 Iterations",
                          "size": "Graph Size",
                          "cca": "Connected Components Version",
                          "density": "Density"},
                  category_orders={"cca": ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"]

                                   })
# fig2.data[1].line.dash = 'dot'
# fig2.data[2].line.dash = 'dot'
for trace in fig2.data:
    trace.line.dash = 'dot'

fig2.update_layout(legend=dict(
    yanchor="top",
    y=0.99,
    xanchor="left",
    x=0.01
))

fig3 = px.scatter(res_high_dens, x='size', y='total_time', color="cca",
                  hover_data=['density'],
                  title="Plot total run time for reference implementation, Density=20",
                  trendline='lowess',
                  log_y=True,
                  color_discrete_map={
                      "1": '#636EFA',
                      "2": '#EF553B',
                      "3": '#00CC96',
                      "4": '#AB63FA',
                      "5": '#FFA15A',
                      "6": '#19D3F3',
                      "7": '#FF6692',
                      "8": '#B6E880'
                  },
                  labels={"total_time": "Time for 1000000 Iterations",
                          "size": "Graph Size",
                          "cca": "Connected Components Version",
                          "density": "Density"},
                  category_orders={"cca": ["1", "2", "3",
                                           "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"]})
for trace in fig3.data:
    trace.line.dash = 'dot'

fig3.update_layout(legend=dict(
    yanchor="top",
    y=0.99,
    xanchor="left",
    x=0.01
))
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

fig6 = px.scatter(mxv_res, x='size', y='mxv_time', color="cca",
                  hover_data=['density'], title="Plot of all 7 Basic Versions",
                  symbol="density",
                  trendline='lowess',
                  color_discrete_map={
                      "1": '#636EFA',
                      "2": '#EF553B',
                      "3": '#00CC96',
                      "4": '#AB63FA',
                      "5": '#FFA15A',
                      "6": '#19D3F3',
                      "7": '#FF6692',
                      "8": '#B6E880'
                  },
                  labels={"mxv_time": "Average over 1000",
                          "size": "Graph Size",
                          "cca": "Connected Components Version",
                          "density": "Density"},
                  category_orders={"cca": ["1", "2", "3", "4", "5", "6", "7"],
                                   "density": ["10", "20"]})

fig7 = px.scatter(mxv_res_low_dens, x='size', y='mxv_time', color="cca",
                  hover_data=['density'],
                  title="Matrix-Vector multiplication time, Density=10",
                  trendline='lowess',
                  color_discrete_map={
                      "1": '#636EFA',
                      "2": '#EF553B',
                      "3": '#00CC96',
                      "4": '#AB63FA',
                      "5": '#FFA15A',
                      "6": '#19D3F3',
                      "7": '#FF6692',
                      "8": '#B6E880'
                  },
                  labels={"mxv_time": "Average time of one MxV in seconds",
                          "size": "Graph Size",
                          "cca": "Connected Components Version",
                          "density": "Density"},
                  category_orders={"cca": ["1", "2", "3", "4", "5", "6", "7"]

                                   })

for trace in fig7.data:
    trace.line.dash = 'dot'
fig7.update_layout(legend=dict(
    yanchor="top",
    y=0.99,
    xanchor="left",
    x=0.01
))

fig8 = px.scatter(mxv_res_high_dens, x='size', y='mxv_time', color="cca",
                  hover_data=['density'],
                  title="Matrix-Vector multiplication time, Density=20",
                  trendline='lowess',
                  color_discrete_map={
                      "1": '#636EFA',
                      "2": '#EF553B',
                      "3": '#00CC96',
                      "4": '#AB63FA',
                      "5": '#FFA15A',
                      "6": '#19D3F3',
                      "7": '#FF6692',
                      "8": '#B6E880'
                  },
                  labels={"mxv_time": "Average time of one MxV in seconds",
                          "size": "Graph Size",
                          "cca": "Connected Components Version",
                          "density": "Density"},
                  category_orders={"cca": ["1", "2", "3",
                                           "4", "5", "6", "7"]})
for trace in fig8.data:
    trace.line.dash = 'dot'

fig8.update_layout(legend=dict(
    yanchor="top",
    y=0.99,
    xanchor="left",
    x=0.01
))

# fig1.show()
fig2.show()
fig3.show()

# Interactive versions
# fig4.show()
# fig5.show()

# fig6.show()
fig7.show()
fig8.show()
