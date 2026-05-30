
COUNT = 10
DEFINE = "#define"
CONCAT1 = "CONCAT_"



out = ""
for i in range(0,COUNT):
    part1 = CONCAT1 + str(i)
    args = "" 
    for j in range(0,i+1):
        args += " T" + str(j)
        if j != i:
            args += ","

    arg_part = "(prefix," + args + ")"

    macro_part = ""
    for j in range(0,i+1):
       if j == 0:
         macro_part += "##"
       macro_part += "T" + str(j)
       if j != i:
          macro_part += "##_"


    main_macro = DEFINE + " " + part1 + arg_part + " " + part1 + "_" + arg_part
    sec_macro  = DEFINE + " " + part1 + "_" + arg_part + " prefix##_" + macro_part
    print(sec_macro)
    print(main_macro)
    print("")
