#include <inttypes.h>
#include "renamer.h"

renamer *REN;

void REN__renamer(uint64_t n_log_regs, uint64_t n_phys_regs, uint64_t n_branches) {
	REN = new renamer(n_log_regs, n_phys_regs, n_branches);
}

bool REN__stall_reg(uint64_t bundle_dst) {
	return( REN->stall_reg(bundle_dst) );
}

bool REN__stall_branch(uint64_t bundle_branch) {
	return( REN->stall_branch(bundle_branch) );
}

uint64_t REN__get_branch_mask() {
	return( REN->get_branch_mask() );
}

uint64_t REN__rename_rsrc(uint64_t log_reg) {
	return( REN->rename_rsrc(log_reg) );
}

uint64_t REN__rename_rdst(uint64_t log_reg) {
	return( REN->rename_rdst(log_reg) );
}

uint64_t REN__checkpoint() {
	return( REN->checkpoint() );
}

bool REN__stall_dispatch(uint64_t bundle_inst) {
	return( REN->stall_dispatch(bundle_inst) );
}

uint64_t REN__dispatch_inst(bool dest_valid, uint64_t log_reg, uint64_t phys_reg, bool load, bool store, bool branch, bool amo, bool csr, uint64_t PC) {
	return( REN->dispatch_inst(dest_valid, log_reg, phys_reg, load, store, branch, amo, csr, PC) );
}

bool REN__is_ready(uint64_t phys_reg) {
	return( REN->is_ready(phys_reg) );
}

void REN__clear_ready(uint64_t phys_reg) {
	REN->clear_ready(phys_reg);
}

void REN__set_ready(uint64_t phys_reg) {
	REN->set_ready(phys_reg);
}

uint64_t REN__read(uint64_t phys_reg) {
	return( REN->read(phys_reg) );
}

void REN__write(uint64_t phys_reg, uint64_t value) {
	REN->write(phys_reg, value);
}

void REN__set_complete(uint64_t AL_index) {
	REN->set_complete(AL_index);
}

void REN__resolve(uint64_t AL_index, uint64_t branch_ID, bool correct) {
	REN->resolve(AL_index, branch_ID, correct);
}

bool REN__precommit(bool &completed,
                    bool &exception, bool &load_viol, bool &br_misp, bool &val_misp,
                    bool &load, bool &store, bool &branch, bool &amo, bool &csr, uint64_t &offending_PC) {
	return( REN->precommit(completed, exception, load_viol, br_misp, val_misp, load, store, branch, amo, csr, offending_PC) );
}

void REN__commit() {
	REN->commit();
}

void REN__squash() {
	REN->squash();
}

bool REN__get_exception(uint64_t AL_index) {
	return( REN->get_exception(AL_index) );
}

void REN__set_exception(uint64_t AL_index) {
	REN->set_exception(AL_index);
}

void REN__set_load_violation(uint64_t AL_index) {
	REN->set_load_violation(AL_index);
}

void REN__set_branch_misprediction(uint64_t AL_index) {
	REN->set_branch_misprediction(AL_index);
}

void REN__set_value_misprediction(uint64_t AL_index) {
	REN->set_value_misprediction(AL_index);
}
