for file in *.xlsx
do 
    # Get filename
    fname=${file%.*}
    # Convert to LaTeX
    ssconvert --export-type=Gnumeric_html:latex $fname.xlsx $fname.tex
    # Substitute longtable to tabular
    sed -i 's/\\begin{longtable}/\\begin{tabular}/g; s/\\end{longtable}/\\end{tabular}/g' $fname.tex 
done
