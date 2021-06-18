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
realcase = []
for file in all_csv_files:
    p = pd.read_csv(file)
    if 'mxv_time' not in p.columns and 'density' not in p.columns:
        realcase.append(p)
    if 'mxv_time' not in p.columns:
        dfs.append(p)
    if 'mxv_time' in p.columns and 'iteration' not in p.columns:
        mxvs.append(p)

result_t = pd.concat(dfs)
temp_list = [x for _, x in result_t.groupby(result_t.cca)]
# result = pd.concat([temp_list[0], temp_list[1],
#                     temp_list[4], temp_list[6], temp_list[7]])
result = pd.concat([temp_list[0]])
result_tt = pd.concat(realcase)


# print(result_tt.duplicated().sum())
# print(result_tt.head())
tmp2 = [x for _, x in result_tt.groupby(result_tt.cca)]
tmp2[1]["total_time"] = tmp2[1]["total_time"]/100
real_result = pd.concat([tmp2[1], tmp2[2]])
# real_result = result_tt

result_t2 = pd.concat(mxvs)
temp_list2 = [x for _, x in result_t2.groupby(result_t2.cca)]
mxv_res = pd.concat([temp_list2[0]])


def return_average(pp):
    gp2 = pp.groupby(pp["size"])
    res_low_22 = gp2.get_group(22)
    mean = res_low_22['total_time'].mean()
    return mean


speedup_list = []
for las in [temp_list[1], temp_list[4], temp_list[6], temp_list[7]]:
    speedup_list.append(return_average(temp_list[0]) / return_average(las))

bar_list = {'cca': ['2', '5', '7', '8'],
            'speedup': speedup_list, 'size': ['22', '22', '22', '22']}
bar_fram = pd.DataFrame(bar_list, columns=['cca', 'speedup', 'size'])


def return_average2(pp):
    gp2 = pp.groupby(pp["size"])
    res_low_22 = gp2.get_group(22)
    mean = res_low_22['mxv_time'].mean()
    return mean


speedup_list2 = []
for las in [temp_list2[1], temp_list2[2], temp_list2[3], temp_list2[4]]:
    speedup_list2.append(return_average2(temp_list2[0]) / return_average2(las))

bar_list_2 = {'cca': ['2', '5', '7', '8'],
              'speedup': speedup_list2, 'size': ['22', '22', '22', '22']}
bar_fram2 = pd.DataFrame(bar_list_2, columns=['cca', 'speedup', 'size'])


# gp2 = res_low_dens.groupby(res_low_dens.size)
# res_low_22 = gp2.get_group(22)

# gp3 = res_high_dens.groupby(res_high_dens.size)
# res_high_22 = gp3.get_group(22)
# average_total_time_low =

average_list2 = [0.0073/(tmp2[1]["total_time"].mean()),
                 0.0073/(tmp2[2]["total_time"].mean())]
bar_list_4 = {'cca': ['V8', 'Ruben'],
              'speedup': average_list2, 'size': ['22', '22']}
bar_fram4 = pd.DataFrame(bar_list_4, columns=['cca', 'speedup', 'size'])

# assume you have a "long-form" data frame
# see https://plotly.com/python/px-arguments/ for more options
# df = pd.read_csv('result2s.csv')

result["cca"] = result["cca"].astype(str)
result["density"] = result["density"].astype(str)

real_result["cca"] = real_result["cca"].astype(str)


mxv_res["cca"] = mxv_res["cca"].astype(str)
mxv_res["density"] = mxv_res["density"].astype(str)
mxv_res["mxv_time"] = mxv_res["mxv_time"]/1000.0

# result2 = result[int(result['cca']) <= 3]
# print(result.head())
grouped = result.groupby(result.density)

res_low_dens = grouped.get_group("10.0")
res_high_dens = grouped.get_group("20.0")


gp = mxv_res.groupby(mxv_res.density)

mxv_res_low_dens = gp.get_group("10")
mxv_res_high_dens = gp.get_group("20")


color_map = {
    "1": '#dd0000',
    "2": '#ee6000',
    "5": '#d000e5',
    "7": '#29dd00',
    "8": '#0053c5',
    "9": '#c50018',
    "V8": '#0053c5',
    "Ruben": '#c50018',
    "Current Hive": '#00c5ad'
}

# print(result.head())
fig1 = px.scatter(result, x='size', y='total_time', color="cca",
                  hover_data=['density'], title="Plot of all 7 Basic Versions",
                  symbol="density",
                  trendline='lowess',
                  log_y=True,
                  color_discrete_map=color_map,
                  labels={"total_time": "Time for 10000 Iterations",
                          "size": "Graph Size",
                          "cca": "CC",
                          "density": "Density"},
                  category_orders={"cca": ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"],
                                   "density": ["10", "20"]})

fig2 = px.scatter(res_low_dens, x='size', y='total_time', color="cca",
                  hover_data=['density'],
                  title="Total runtime, Density=10",
                  trendline='lowess',
                  log_y=True,
                  color_discrete_map=color_map,
                  labels={"total_time": "Time for 1000000 Iterations",
                          "size": "Graph Size",
                          "cca": "CC",
                          "density": "Density"},
                  category_orders={"cca": ["V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8"]
                                   })


fig3 = px.scatter(res_high_dens, x='size', y='total_time', color="cca",
                  hover_data=['density'],
                  title="Total runtime, Density=20",
                  trendline='lowess',
                  log_y=True,
                  color_discrete_map=color_map,
                  labels={"total_time": "Time for 1000000 Iterations",
                          "size": "Graph Size",
                          "cca": "CC",
                          "density": "Density"},
                  category_orders={"cca": ["V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8"]})


fig6 = px.scatter(mxv_res, x='size', y='mxv_time', color="cca",
                  hover_data=['density'], title="Plot of all 7 Basic Versions",
                  symbol="density",
                  trendline='lowess',
                  color_discrete_map=color_map,
                  labels={"mxv_time": "Average over 1000",
                          "size": "Graph Size",
                          "cca": "CC",
                          "density": "Density"},
                  category_orders={"cca": ["V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8"],
                                   "density": ["10", "20"]})

fig7 = px.scatter(mxv_res_low_dens, x='size', y='mxv_time', color="cca",
                  hover_data=['density'],

                  title="Matrix-Vector multiplication time, Density=10",
                  trendline='lowess',

                  color_discrete_map=color_map,
                  labels={"mxv_time": "Average time of one MxV in seconds",
                          "size": "Graph Size",
                          "cca": "CC",
                          "density": "Density"},
                  category_orders={"cca": ["V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8"]

                                   })


fig8 = px.scatter(mxv_res_high_dens, x='size', y='mxv_time', color="cca",
                  hover_data=['density'],
                  title="Matrix-Vector multiplication time, Density=20",
                  trendline='lowess',
                  color_discrete_map=color_map,
                  labels={"mxv_time": "Average time of one MxV in seconds",
                          "size": "Graph Size",
                          "cca": "CC",
                          "density": "Density"},
                  category_orders={"cca": ["V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8"]})


fig9 = px.scatter(real_result, x='size', y='total_time', color="cca",
                  title="Plot of total execution time for real hive game graphs",
                  color_discrete_map=color_map,
                  log_y=True,
                  labels={"total_time": "Time for 1000000 Iterations",
                          "size": "Graph Size",
                          "cca": "CC"},
                  category_orders={"cca": ["V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "Rubens Version"]})


fig10 = px.bar(bar_fram, x='size', y='speedup', color='cca',
               title="The average speedup for graph size 22 for total execution time", barmode='group',
               color_discrete_map=color_map,
               labels={"speedup": "Average speedup against the reference",
                       "size": "Graph Size",
                       "cca": "CC"},
               category_orders={"cca": ["V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8"]})


fig11 = px.bar(bar_fram2, x='size', y='speedup', color='cca',
               title="The average speedup for graph size 22 for matrix-vector multiplication", barmode='group',
               color_discrete_map=color_map,
               labels={"speedup": "Average speedup against the reference",
                       "size": "Graph Size",
                       "cca": "CC"},
               category_orders={"cca": ["V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8"]})


fig13 = px.bar(bar_fram4, x='size', y='speedup', color='cca',
               title="The average speedup for graph size 22 for real hive graphs against current Hive version", barmode='group',
               color_discrete_map=color_map,
               labels={"speedup": "Average speedup of total runtime",
                       "size": "Graph Size",
                       "cca": "CC"},
               category_orders={"cca": ["V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8"]})


fig_list = [fig2, fig3, fig7, fig8, fig9, fig10, fig11]
fig_list_2 = [fig2, fig3, fig7, fig8, fig9]

for fig in fig_list:
    for trace in fig.data:
        if trace.name == "9":
            trace.name = "Ruben"
        else:
            trace.name = "V" + trace.name
    fig.update_layout({'plot_bgcolor': 'rgba(0, 0, 0, 0)'
                       })
    fig.update_xaxes(showline=True, linewidth=1, linecolor='grey',
                     mirror=True, gridwidth=1, gridcolor='grey')
    fig.update_yaxes(showline=True, linewidth=1, linecolor='grey',
                     mirror=True, gridwidth=1, gridcolor='grey')


for fig in fig_list_2:
    fig.update_xaxes(range=[9.75, 32.25], constrain="domain", nticks=10)
    for trace in fig.data:
        trace.line.dash = 'dot'

fig9.update_xaxes(range=[0, 22.25])


fig13.update_layout({'plot_bgcolor': 'rgba(0, 0, 0, 0)'
                     })
fig13.update_xaxes(showline=True, linewidth=1, linecolor='grey',
                   mirror=True, gridwidth=1, gridcolor='grey')
fig13.update_yaxes(showline=True, linewidth=1, linecolor='grey',
                   mirror=True, gridwidth=1, gridcolor='grey')

fig2.write_image("./Results/Graphsdump/pdfs/cc1_d10.pdf")
fig3.write_image("./Results/Graphsdump/pdfs/cc1_d20.pdf")
fig7.write_image("./Results/Graphsdump/pdfs/mxv_cc1_d10.pdf")
fig8.write_image("./Results/Graphsdump/pdfs/mxv_cc1_d20.pdf")


# fig9.write_image("./Results/Graphsdump/pdfs/real_result.pdf")
# fig10.write_image("./Results/Graphsdump/pdfs/speedup.pdf")
# fig11.write_image("./Results/Graphsdump/pdfs/speedup_mxv.pdf")
# fig13.write_image("./Results/Graphsdump/pdfs/speedup_real.pdf")

# fig1.show()
# fig2.show()
# fig3.show()

print("Fertig")
# # fig6.show()
# fig7.show()
# fig8.show()

# fig9.show()
# fig10.show()
# fig11.show()


# fig13.show()
