import os

model_list = ["perfALL","realDS","realBP","noTS","rdn","rds","rdsc"]
for model in model_list:
    for i in range(1,5):
        print(model+" "+str(i))
        os.system("cat {}.config{} | grep \"ipc\"".format(model,i))
    print("")
#os.system("cat perfALL.config2 | grep \"ipc\"")
#os.system("cat perfALL.config3 | grep \"ipc\"")
#os.system("cat perfALL.config4 | grep \"ipc\"")
