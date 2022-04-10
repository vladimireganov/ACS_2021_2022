
file_name = "april_log.txt"
out_file_name = "servo_control.txt"

if __name__ == "__main__":

    with open(out_file_name, "w") as out_file:
        with open(file_name, "r") as log_file:
            line = log_file.readline()
            previous_time = float(line.split('\t')[1])
            current_time = previous_time

            while(line):
                message = line.split('\t')[2]
                m_list = message.split()

                print(m_list)

                if len(m_list) >= 2 and m_list[1] == "angle":
                    previous_time = current_time
                    current_time = float(line.split('\t')[1])
                    elapsed_time = current_time - previous_time
                    out_file.write(str(elapsed_time) + " " + m_list[2] + "\n")

                line = log_file.readline()
