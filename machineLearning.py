url = "https://raw.githubusercontent.com/Shivvamm04/Sales_pridiction/main/irrigation.csv"
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
df = pd.read_csv(url)
df.head()
df.shape
df.corr()
df.plot(kind = 'scatter',x = "Water level", y = "Moter O/P")
df.plot(kind = 'scatter',x = "Humidity", y = "Moter O/P")
df.plot(kind = 'scatter',x = "Moisture", y = "Moter O/P")
from sklearn.model_selection import train_test_split
X_train,X_test= train_test_split(df,test_size = 0.2, random_state = 42)
df = X_train.drop("Moter O/P",axis = 1)
df_op = X_train["Moter O/P"].copy()
from sklearn.pipeline import Pipeline
from sklearn.impute import SimpleImputer
from sklearn.preprocessing import StandardScaler
my_pipeline = Pipeline([
 ('imputer', SimpleImputer(strategy="median")),
 ('std_scaler', StandardScaler()),
])
df.shape
from sklearn.linear_model import LinearRegression
from sklearn.tree import DecisionTreeRegressor
from sklearn.ensemble import RandomForestRegressor
model = RandomForestRegressor()
Page | 49
model.fit(df,df_op)
some_data = df.iloc[:5]
some_labels = df_op.iloc[:5]
model.predict(some_data)
list(some_labels)
from sklearn.metrics import mean_squared_error
df_predictions = model.predict(df)
mse = mean_squared_error(df_op, df_predictions)
rmse = np.sqrt(mse)
rmse
from sklearn.model_selection import cross_val_score
scores = cross_val_score(model, df, df_op, scoring="neg_mean_squared_error", cv=10)
rmse_scores = np.sqrt(-scores)
rmse_scores
def print_scores(scores):
 print("Scores:", scores)
 print("Mean: ", scores.mean())
 print("Standard deviation: ", scores.std())
print_scores(rmse_scores)
from joblib import dump, load
dump(model, 'irrigation.joblib')
X_t = X_test.drop("Moter O/P", axis=1)
Y_t = X_test["Moter O/P"].copy()
# X_test_prepared = my_pipeline.transform(X_t)
final_predictions = model.predict(X_t)
final_mse = mean_squared_error(Y_t, final_predictions)
final_rmse = np.sqrt(final_mse)
final_rmse
# df[0] //prepared_data
dt = [[40, 440, 39, 83, 0]]
# ch_d = my_pipeline.transform(dt)
#ch_d[0]
