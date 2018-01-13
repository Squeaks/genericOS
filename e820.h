struct e820_entry {
  uint64_t base;
  uint64_t region_len;
  uint32_t type;
  uint32_t acpi_ext;
};

void print_e820_map(void *);
