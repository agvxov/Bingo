for i in $(seq 1 1 5); do
	for h in $(seq 1 1 5); do
		echo "\
    <item row=\"$i\" column=\"$h\">
     <widget class=\"QPushButton\" name=\"pushButton_$(expr \( \( $i - 1 \) '*' 5 \) + $h)\">
      <property name=\"minimumSize\">
       <size>
        <width>100</width>
        <height>100</height>
       </size>
      </property>
      <property name=\"maximumSize\">
       <size>
        <width>100</width>
        <height>100</height>
       </size>
      </property>
      <property name=\"text\">
       <string>PushButton</string>
      </property>
      <property name=\"checkable\">
       <bool>true</bool>
      </property>
     </widget>
    </item>"
	done
done


#for i in $(seq 0 1 4); do
#	for h in $(seq 0 1 4); do
#		echo "	this->bingo_grid[$i][$h] = ui->pushButton_$(expr $i '*' 5 + $h + 1);"
#	done
#done

#for i in $(seq 0 1 25); do
#	echo "	this->bingo_grid[$i] = ui->pushButton_$(expr $i + 1);"
#done
