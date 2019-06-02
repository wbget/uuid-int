export as namespace uuidInt;

export = UUID;

declare function UUID(id: number): UUID.Generator;

declare namespace UUID {
  export interface Generator {
    id: number;
    uuid(): number;
  }
}
