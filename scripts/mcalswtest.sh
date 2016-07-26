cd $MCALSW/bin
echo " MCALSW TEST:"
echo "... modifying default environment..."
mv mcalsw_output mcalsw_output_GOOD
ln -s /data1/AGILE_MCAL_AIV_Data/MCALSW_test/ mcalsw_output

cd mcalsw_output
echo "... testing SW functionalities..."
for filename in *_3909_000.lv1 *_3902_000.lv1  *_3901_000.lv1 *_3908_000.lv1 *_3910_000.lv1
do
        echo ...working on file $filename
	mcalanalyzer $filename 
done

for filename in *_3201_000.lv1
do
        hkmonitor $filename tmp.root
done
hkviewer tmp.root&

viewrun ./H/H010548_3909_sel_01.root &
viewrun ./H/H010560_3902.root &

echo "... restoring default environment..."
cd $MCALSW/bin
rm mcalsw_output
mv mcalsw_output_GOOD mcalsw_output
