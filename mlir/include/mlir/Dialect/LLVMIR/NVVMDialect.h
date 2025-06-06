//===- NVVMDialect.h - MLIR NVVM IR dialect ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the NVVM IR dialect in MLIR, containing NVVM operations and
// NVVM specific extensions to the LLVM type system.
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_DIALECT_LLVMIR_NVVMDIALECT_H_
#define MLIR_DIALECT_LLVMIR_NVVMDIALECT_H_

#include "mlir/Bytecode/BytecodeOpInterface.h"
#include "mlir/Dialect/GPU/IR/GPUDialect.h"
#include "mlir/Dialect/LLVMIR/BasicPtxBuilderInterface.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Dialect/LLVMIR/NVVMRequiresSMTraits.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Interfaces/InferIntRangeInterface.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include "mlir/Target/LLVMIR/ModuleTranslation.h"
#include "llvm/IR/IntrinsicsNVPTX.h"

#include "mlir/Dialect/LLVMIR/NVVMOpsEnums.h.inc"

namespace mlir {
namespace NVVM {

// Shared memory has 128-bit alignment
constexpr int kSharedMemoryAlignmentBit = 128;

/// NVVM memory space identifiers.
enum NVVMMemorySpace {
  /// Global memory space identifier.
  kGlobalMemorySpace = 1,
  /// Shared memory space identifier.
  kSharedMemorySpace = 3,
  /// Constant memory space identifier.
  kConstantMemorySpace = 4,
  /// Tensor memory space identifier.
  /// Tensor memory is available only in arch-accelerated
  /// variants from sm100 onwards.
  kTensorMemorySpace = 6,
  /// Distributed shared memory space identifier.
  /// Distributed shared memory is available only in sm90+.
  kSharedClusterMemorySpace = 7,
};

/// A pair type of LLVM's Intrinsic ID and args (which are llvm values).
/// This type is returned by the getIntrinsicIDAndArgs() methods.
using IDArgPair =
    std::pair<llvm::Intrinsic::ID, llvm::SmallVector<llvm::Value *>>;

/// Return the element type and number of elements associated with a wmma matrix
/// of given chracteristics. This matches the logic in IntrinsicsNVVM.td
/// WMMA_REGS structure.
std::pair<mlir::Type, unsigned> inferMMAType(mlir::NVVM::MMATypes type,
                                             mlir::NVVM::MMAFrag frag, int nRow,
                                             int nCol,
                                             mlir::MLIRContext *context);
} // namespace NVVM
} // namespace mlir

///// Ops /////
#define GET_ATTRDEF_CLASSES
#include "mlir/Dialect/LLVMIR/NVVMOpsAttributes.h.inc"

#define GET_OP_CLASSES
#include "mlir/Dialect/LLVMIR/NVVMOps.h.inc"

#include "mlir/Dialect/LLVMIR/NVVMOpsDialect.h.inc"

#endif /* MLIR_DIALECT_LLVMIR_NVVMDIALECT_H_ */
