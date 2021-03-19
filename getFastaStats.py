import pyfastx

fa=pyfastx.Fasta('/Users/nouratbeileh/Downloads/ecoli-reads.fasta')

print(len(fa.longest))