# DEFL8

Defl8 is a DEFLATE-based compression and decompression that use Huffman and LZ78.

## Compile (recommended Linux environment)

```bash
cd defl8
sh tools/build.sh
```

## Usage

### Compression

```
# ./defl8 -c input-file output-file.defl8
```

### Decompression

```
# ./defl8 -d input-file.defl8 output-file
```