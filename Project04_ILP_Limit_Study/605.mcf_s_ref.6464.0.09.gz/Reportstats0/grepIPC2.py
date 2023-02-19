import os


print("\n\n")

model_list = ["realDS","realBP","noTS","rd*"]
for i in range(1,5): 
    for model in model_list:
        var = os.popen("cat {}.config{} | grep \"ipc\"".format(model,i)).read()
        print( "config{} ".format(i)+model+" "+var  )
    print("")
#os.system("cat perfALL.config2 | grep \"ipc\"")
#os.system("cat perfALL.config3 | grep \"ipc\"")
#os.system("cat perfALL.config4 | grep \"ipc\"")
